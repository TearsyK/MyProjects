using System;
using System.Drawing;
using System.Drawing.Imaging;
using System.Runtime.InteropServices;
using System.Windows.Forms;
using System.Diagnostics;
using System.IO;
using System.Collections.Generic;
using System.Threading.Tasks;

namespace GaussianBlurUI
{
    public partial class Form1 : Form
    {
        // ====================================================================
        // IMPORTY DLL
        // ====================================================================

        // ========== ASSEMBLER DLL ==========
        [DllImport("GaussianBlurASM_Horizontal.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void GaussianBlurASM_Horizontal(
            byte[] inputImage,
            byte[] tempBuffer,
            int width,
            int stride,
            int startY,
            int endY
        );

        [DllImport("GaussianBlurASM_Vertical.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void GaussianBlurASM_Vertical(
            byte[] tempBuffer,
            byte[] outputImage,
            int width,
            int stride,
            int startY,
            int endY,
            int height
        );

        // ========== C++ DLL  ==========
        [DllImport("GaussianBlurCPP_Horizontal.dll", CallingConvention = CallingConvention.Cdecl,
                   EntryPoint = "GaussianBlurCPP_Horizontal")]
        private static extern void GaussianBlurCPP_Horizontal(
            byte[] inputImage,
            byte[] tempBuffer,
            int width,
            int stride,
            int startY,
            int endY
        );

        [DllImport("GaussianBlurCPP_Vertical.dll", CallingConvention = CallingConvention.Cdecl,
                   EntryPoint = "GaussianBlurCPP_Vertical")]
        private static extern void GaussianBlurCPP_Vertical(
            byte[] tempBuffer,
            byte[] outputImage,
            int width,
            int stride,
            int startY,
            int endY,
            int height
        );

        // ====================================================================
        // ZMIENNE
        // ====================================================================

        private Bitmap inputBitmap = null;
        private Bitmap outputBitmap = null;
        private int maxThreadsAvailable;

        // ====================================================================
        // KONSTRUKTOR
        // ====================================================================

        public Form1()
        {
            InitializeComponent();

            maxThreadsAvailable = Environment.ProcessorCount;
            numThreads.Minimum = 1;
            numThreads.Maximum = Math.Min(64, maxThreadsAvailable);
            numThreads.Value = maxThreadsAvailable;

            btnProcess.Enabled = false;
            lblStatus.Text = $"Gotowy ({maxThreadsAvailable} wątków dostępnych)";
            lblStatus.ForeColor = Color.Green;
            this.Text = "Rozmycie Gaussa - Q7 Fixed-Point (Kernel 3×3)";

            // USUNIĘTE: numSigma, lblSigma (nie potrzebne dla Q7)
        }

        // ====================================================================
        // WCZYTYWANIE OBRAZU
        // ====================================================================

        private void btnLoadImage_Click(object sender, EventArgs e)
        {
            using (OpenFileDialog dlg = new OpenFileDialog())
            {
                dlg.Filter = "Obrazy|*.jpg;*.jpeg;*.png;*.bmp|Wszystkie pliki|*.*";
                dlg.Title = "Wybierz obraz do przetworzenia";

                if (dlg.ShowDialog() == DialogResult.OK)
                {
                    try
                    {
                        if (inputBitmap != null)
                            inputBitmap.Dispose();

                        inputBitmap = new Bitmap(dlg.FileName);

                        if (inputBitmap.PixelFormat != PixelFormat.Format24bppRgb)
                        {
                            Bitmap converted = new Bitmap(inputBitmap.Width, inputBitmap.Height,
                                PixelFormat.Format24bppRgb);
                            using (Graphics g = Graphics.FromImage(converted))
                            {
                                g.DrawImage(inputBitmap, 0, 0, inputBitmap.Width, inputBitmap.Height);
                            }
                            inputBitmap.Dispose();
                            inputBitmap = converted;
                        }

                        pictureBoxInput.Image = inputBitmap;

                        if (outputBitmap != null)
                        {
                            outputBitmap.Dispose();
                            outputBitmap = null;
                        }
                        pictureBoxOutput.Image = null;

                        btnProcess.Enabled = true;
                        lblStatus.Text = $"Wczytano: {inputBitmap.Width}×{inputBitmap.Height} px";
                        lblStatus.ForeColor = Color.Green;
                        lblExecutionTime.Text = "";
                    }
                    catch (Exception ex)
                    {
                        MessageBox.Show($"Błąd wczytywania obrazu:\n{ex.Message}",
                            "Błąd", MessageBoxButtons.OK, MessageBoxIcon.Error);
                        lblStatus.Text = "Błąd wczytywania!";
                        lblStatus.ForeColor = Color.Red;
                    }
                }
            }
        }

        // ====================================================================
        // PRZETWARZANIE - WIELOWĄTKOWE
        // ====================================================================

        private void ProcessImageMultiThreaded(
            byte[] inputImageData,
            byte[] outputImageData,
            int width,
            int height,
            int stride,
            int numThreads,
            bool useASM,
            Stopwatch sw)
        {
            byte[] tempBuffer = new byte[stride * height];
            List<Task> tasks = new List<Task>();

            // ================================================================
            // PRE-KALKULACJA ZAKRESÓW DLA HORIZONTAL
            // ================================================================
            var horizontalRanges = new (int threadId, int startY, int endY)[numThreads];
            for (int t = 0; t < numThreads; ++t)
            {
                horizontalRanges[t] = (
                    threadId: t,
                    startY: CalculateStartY_Horizontal(t, height, numThreads),
                    endY: CalculateEndY_Horizontal(t, height, numThreads)
                );
            }

            // ================================================================
            // FAZA 1: KONWOLUCJA POZIOMA
            // ================================================================
            sw.Start();
            foreach (var range in horizontalRanges)
            {
                Task task = Task.Run(() =>
                {
                    try
                    {
                        if (useASM)
                        {
                            GaussianBlurASM_Horizontal(
                                inputImageData, tempBuffer, width, stride, range.startY, range.endY
                            );
                        }
                        else
                        {
                            GaussianBlurCPP_Horizontal(
                                inputImageData, tempBuffer, width, stride, range.startY, range.endY
                            );
                        }
                    }
                    catch (Exception ex)
                    {
                        throw new Exception($"Błąd w wątku {range.threadId} (Horizontal): {ex.Message}", ex);
                    }
                });

                tasks.Add(task);
            }
            

            Task.WaitAll(tasks.ToArray());
            sw.Stop();
            tasks.Clear();

            // ================================================================
            // PRE-KALKULACJA ZAKRESÓW DLA VERTICAL
            // ================================================================
            var verticalRanges = new (int threadId, int startY, int endY)[numThreads];
            for (int t = 0; t < numThreads; ++t)
            {
                verticalRanges[t] = (
                    threadId: t,
                    startY: CalculateStartY_Vertical(t, height, numThreads),
                    endY: CalculateEndY_Vertical(t, height, numThreads)
                );
            }

            // ================================================================
            // FAZA 2: KONWOLUCJA PIONOWA
            // ================================================================
            sw.Start();
            foreach (var range in verticalRanges)
            {
                 Task task = Task.Run(() =>
                { 
                    try
                    {
                        if (useASM)
                        {
                            GaussianBlurASM_Vertical(
                                tempBuffer, outputImageData, width, stride, range.startY, range.endY, height
                            );
                        }
                        else
                        {
                            GaussianBlurCPP_Vertical(
                                tempBuffer, outputImageData, width, stride, range.startY, range.endY, height
                            );
                        }
                    }
                    catch (Exception ex)
                    {
                        throw new Exception($"Błąd w wątku {range.threadId} (Vertical): {ex.Message}", ex);
                    }
                });

                tasks.Add(task);
            }
            

            Task.WaitAll(tasks.ToArray());
            sw.Stop();
        }


        // ==================== HORIZONTALNE (bez nakładu) ====================
        private int CalculateStartY_Horizontal(int threadIndex, int height, int numThreads)
        {
            int baseSize = height / numThreads;
            int remainder = height % numThreads;

            int startY = 0;
            for (int i = 0; i < threadIndex; ++i)
            {
                int size = baseSize + (i < remainder ? 1 : 0);
                startY += size;
            }

            return startY;
        }

        private int CalculateEndY_Horizontal(int threadIndex, int height, int numThreads)
        {
            int baseSize = height / numThreads;
            int remainder = height % numThreads;

            int endY = 0;
            for (int i = 0; i <= threadIndex; ++i)
            {
                int size = baseSize + (i < remainder ? 1 : 0);
                endY += size;
            }

            return endY;
        }

        // ==================== VERTYKALNE (z nakładem) ====================

        private int CalculateStartY_Vertical(int threadIndex, int height, int numThreads)
        {
            if (threadIndex == 0)
                return 0;

            int prevThreadEndY = CalculateEndY_Horizontal(threadIndex - 1, height, numThreads);
            return prevThreadEndY - 1;
        }

        private int CalculateEndY_Vertical(int threadIndex, int height, int numThreads)
        {
            int baseSize = height / numThreads;
            int remainder = height % numThreads;

            int endY = 0;
            for (int i = 0; i <= threadIndex; ++i)
            {
                int size = baseSize + (i < remainder ? 1 : 0);
                endY += size;
            }

            return endY;
        }

        // ====================================================================
        // PRZETWARZANIE
        // ====================================================================

        private void btnProcess_Click(object sender, EventArgs e)
        {
            if (inputBitmap == null)
            {
                MessageBox.Show("Najpierw wczytaj obraz!", "Brak obrazu",
                    MessageBoxButtons.OK, MessageBoxIcon.Warning);
                return;
            }

            btnLoadImage.Enabled = false;
            btnProcess.Enabled = false;
            radioCPP.Enabled = false;
            radioASM.Enabled = false;
            numThreads.Enabled = false;
            progressBar.Visible = true;
            progressBar.Style = ProgressBarStyle.Marquee;

            lblStatus.Text = "Przetwarzanie...";
            lblStatus.ForeColor = Color.Blue;
            lblExecutionTime.Text = "";

            Application.DoEvents();

            int width = inputBitmap.Width;
            int height = inputBitmap.Height;
            int threads = (int)numThreads.Value;
            bool useASM = radioASM.Checked;

            try
            {
                if (outputBitmap != null)
                    outputBitmap.Dispose();

                outputBitmap = new Bitmap(width, height, PixelFormat.Format24bppRgb);

                BitmapData inData = null;
                BitmapData outData = null;

                try
                {
                    inData = inputBitmap.LockBits(
                        new Rectangle(0, 0, width, height),
                        ImageLockMode.ReadOnly,
                        PixelFormat.Format24bppRgb);

                    outData = outputBitmap.LockBits(
                        new Rectangle(0, 0, width, height),
                        ImageLockMode.WriteOnly,
                        PixelFormat.Format24bppRgb);

                    byte[] inputData = new byte[Math.Abs(inData.Stride) * height];
                    byte[] outputData = new byte[Math.Abs(outData.Stride) * height];
                    Marshal.Copy(inData.Scan0, inputData, 0, inputData.Length);

                    int stride = Math.Abs(inData.Stride);

                    Stopwatch sw = Stopwatch.StartNew();
                    sw.Stop();
                    ProcessImageMultiThreaded(
                        inputData, outputData, width, height, stride, threads, useASM, sw
                    );

                    

                    Marshal.Copy(outputData, 0, outData.Scan0, outputData.Length);

                    string impl = useASM ? "ASM x64" : "C++ Q7";
                    double ms = sw.Elapsed.TotalMilliseconds;
                    lblExecutionTime.Text = $"Czas: {ms:F2} ms ({impl}, {threads} wątków)";
                    lblExecutionTime.ForeColor = Color.DarkGreen;

                    lblStatus.Text = $"Sukces!";
                    lblStatus.ForeColor = Color.Green;
                }
                finally
                {
                    if (inData != null)
                    {
                        try { inputBitmap.UnlockBits(inData); }
                        catch { }
                    }

                    if (outData != null)
                    {
                        try { outputBitmap.UnlockBits(outData); }
                        catch { }
                    }
                }

                pictureBoxOutput.Image = outputBitmap;
            }
            catch (DllNotFoundException dllEx)
            {
                MessageBox.Show(
                    $"Nie znaleziono biblioteki DLL!\n\n" +
                    $"Błąd: {dllEx.Message}\n\n" +
                    $"Wymagane pliki:\n" +
                    $"- GaussianBlurASM_Horizontal.dll\n" +
                    $"- GaussianBlurASM_Vertical.dll\n" +
                    $"- GaussianBlurCPP_Horizontal.dll\n" +
                    $"- GaussianBlurCPP_Vertical.dll",
                    "Błąd DLL", MessageBoxButtons.OK, MessageBoxIcon.Error);

                lblStatus.Text = "✗ Błąd: Brak DLL!";
                lblStatus.ForeColor = Color.Red;
            }
            catch (Exception ex)
            {
                MessageBox.Show(
                    $"Błąd przetwarzania:\n\n{ex.Message}\n\n" +
                    $"Stack trace:\n{ex.StackTrace}",
                    "Błąd", MessageBoxButtons.OK, MessageBoxIcon.Error);

                lblStatus.Text = "✗ Błąd przetwarzania!";
                lblStatus.ForeColor = Color.Red;

                if (outputBitmap != null)
                {
                    outputBitmap.Dispose();
                    outputBitmap = null;
                }
            }
            finally
            {
                btnLoadImage.Enabled = true;
                btnProcess.Enabled = true;
                radioCPP.Enabled = true;
                radioASM.Enabled = true;
                numThreads.Enabled = true;
                progressBar.Visible = false;
            }
        }

        // ====================================================================
        // ZAPISYWANIE WYNIKU
        // ====================================================================

        private void btnSave_Click(object sender, EventArgs e)
        {
            if (outputBitmap == null)
            {
                MessageBox.Show("Najpierw przetwórz obraz!", "Brak wyniku",
                    MessageBoxButtons.OK, MessageBoxIcon.Warning);
                return;
            }

            using (SaveFileDialog dlg = new SaveFileDialog())
            {
                dlg.Filter = "PNG|*.png|JPEG|*.jpg|BMP|*.bmp";
                dlg.Title = "Zapisz przetworzony obraz";
                dlg.FileName = "gaussian_blur_q7_result.png";

                if (dlg.ShowDialog() == DialogResult.OK)
                {
                    try
                    {
                        ImageFormat format = ImageFormat.Png;
                        string ext = Path.GetExtension(dlg.FileName).ToLower();

                        if (ext == ".jpg" || ext == ".jpeg")
                            format = ImageFormat.Jpeg;
                        else if (ext == ".bmp")
                            format = ImageFormat.Bmp;

                        outputBitmap.Save(dlg.FileName, format);

                        lblStatus.Text = $"Zapisano: {Path.GetFileName(dlg.FileName)}";
                        lblStatus.ForeColor = Color.Green;

                        MessageBox.Show($"Obraz zapisano pomyślnie:\n{dlg.FileName}",
                            "Sukces", MessageBoxButtons.OK, MessageBoxIcon.Information);
                    }
                    catch (Exception ex)
                    {
                        MessageBox.Show($"Błąd zapisu:\n{ex.Message}",
                            "Błąd", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    }
                }
            }
        }

        // ====================================================================
        // CZYSZCZENIE ZASOBÓW
        // ====================================================================

        protected override void OnFormClosing(FormClosingEventArgs e)
        {
            if (inputBitmap != null)
                inputBitmap.Dispose();

            if (outputBitmap != null)
                outputBitmap.Dispose();

            base.OnFormClosing(e);
        }
    }
}
