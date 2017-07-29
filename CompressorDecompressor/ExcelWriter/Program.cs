using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Excel = Microsoft.Office.Interop.Excel;

namespace ExcelWriter
{
    public class Entry
    {
        public int BitCount { get; set; }
        public int FileSize { get; set; }
        public double RMS { get; set; }
    }

    class Program
    {
        static int Main(string[] args)
        {
            if (args.Length != 2)
            {
                return -1;
            }

            var data = new List<Entry>()
            {
                new Entry { BitCount = 1, FileSize = 0, RMS = 0 },
                new Entry { BitCount = 2, FileSize = 0, RMS = 0 },
                new Entry { BitCount = 3, FileSize = 0, RMS = 0 },
                new Entry { BitCount = 4, FileSize = 0, RMS = 0 }
            };

            string line;
            string[] entryStrings;
            System.IO.StreamReader file = new System.IO.StreamReader(args[0]);
            while ((line = file.ReadLine()) != null)
            {
                entryStrings = line.Split(';');
                data.Add(new Entry { BitCount = int.Parse(entryStrings[0]), FileSize = int.Parse(entryStrings[1]), RMS = double.Parse(entryStrings[2]) });
            }

            DisplayInExcel(data, args[1]);

            return 0;
        }

        static void DisplayInExcel(IEnumerable<Entry> entries, string saveLocation)
        {
            var excelApp = new Excel.Application();
            excelApp.Visible = true;
            excelApp.Workbooks.Add();

            Excel._Worksheet worksheet = (Excel.Worksheet)excelApp.ActiveSheet;
            worksheet.Name = "BitdepthRMSAnalysis";

            worksheet.Cells[1, "B"] = "Bit Depth";
            worksheet.Cells[1, "C"] = "File Size";
            worksheet.Cells[1, "D"] = "RMS";

            var row = 1;
            foreach (var entry in entries)
            {
                row++;
                worksheet.Cells[row, "B"] = entry.BitCount;
                worksheet.Cells[row, "C"] = entry.FileSize;
                worksheet.Cells[row, "D"] = entry.RMS;
            }

            worksheet.Columns[2].AutoFit();
            worksheet.Columns[3].AutoFit();
            worksheet.Columns[4].AutoFit();

            Excel.Range dataRange = worksheet.Range["B1", "D" + row.ToString()];

            excelApp.Charts.Add();
            Excel._Chart chart = excelApp.ActiveChart;
            chart.Name = "Scatter RMS Visualizer";

            chart.SetSourceData(dataRange, Excel.XlRowCol.xlColumns);
            chart.ChartType = Excel.XlChartType.xlXYScatterLines;
            chart.ChartWizard(
                dataRange,
                Excel.XlChartType.xlXYScatterSmooth,
                Type.Missing,
                Excel.XlRowCol.xlColumns,
                Type.Missing,
                Type.Missing,
                true,
                "File size and RMS error along increasing bitdepth",
                "Bit depth",
                Type.Missing,
                Type.Missing
                );

            // Bit depth
            Excel.Series s = chart.SeriesCollection(1);
            s.Delete();

            //// RMS
            s = chart.SeriesCollection(2);
            s.AxisGroup = Excel.XlAxisGroup.xlSecondary;

            Excel.Axis a = chart.Axes(Excel.XlAxisGroup.xlPrimary);
            a.MajorUnit = 1;
            a.MinorUnit = 0.2;
            a.HasMajorGridlines = true;
            a.MinimumScale = 5;
            a.MaximumScale = 16;

            a = chart.Axes(Excel.XlAxisGroup.xlSecondary);
            a.MinimumScale = 0;
            

            Excel._Workbook wb = excelApp.ActiveWorkbook;
            wb.SaveAs(saveLocation);
        }
    }
}
