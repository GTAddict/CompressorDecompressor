# CompressorDecompressor
Program that would perform lossy compression (to the specified bit depth) and another that could decompress the compressed files.

There are three programs here. One that will read the input vertices and compress it to the bit depth specified. The second one will read the compressed data. The ExcelWriter will take this data and plot a graph comparing the loss.

Finally there are batch files to build and execute both programs. The MasterExecute will automate the entire process of building, running the programs with all the bit depths, and then generating and opening the excel file.
