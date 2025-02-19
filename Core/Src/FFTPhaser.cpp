/*
 * FFTPhaser.cpp
 *
 *  Created on: Feb 19, 2025
 *      Author: hayden
 */
#include "FFTPhaser.h"

FFTPhaser::FFTPhaser() {
	// calculate the twiddle coefficients
	for (uint16_t i = 0; i < FFT_SIZE; ++i) {
		twiddleCoef[2 * i] = std::cos(
				(float) i * DSP::twoPi / (float) FFT_SIZE);
		twiddleCoef[2 * i + 1] = std::sin(
				(float) i * DSP::twoPi / (float) FFT_SIZE);

	}

	// hard code these
	revIndexTable = {
			//8x4, size 440
			8, 512, 16, 1024, 24, 1536, 32, 64, 40, 576, 48, 1088, 56, 1600, 64,
			128, 72, 640, 80, 1152, 88, 1664, 96, 192, 104, 704, 112, 1216, 120,
			1728, 128, 256, 136, 768, 144, 1280, 152, 1792, 160, 320, 168, 832,
			176, 1344, 184, 1856, 192, 384, 200, 896, 208, 1408, 216, 1920, 224,
			448, 232, 960, 240, 1472, 248, 1984, 256, 512, 264, 520, 272, 1032,
			280, 1544, 288, 640, 296, 584, 304, 1096, 312, 1608, 320, 768, 328,
			648, 336, 1160, 344, 1672, 352, 896, 360, 712, 368, 1224, 376, 1736,
			384, 520, 392, 776, 400, 1288, 408, 1800, 416, 648, 424, 840, 432,
			1352, 440, 1864, 448, 776, 456, 904, 464, 1416, 472, 1928, 480, 904,
			488, 968, 496, 1480, 504, 1992, 520, 528, 512, 1024, 528, 1040, 536,
			1552, 544, 1152, 552, 592, 560, 1104, 568, 1616, 576, 1280, 584,
			656, 592, 1168, 600, 1680, 608, 1408, 616, 720, 624, 1232, 632,
			1744, 640, 1032, 648, 784, 656, 1296, 664, 1808, 672, 1160, 680,
			848, 688, 1360, 696, 1872, 704, 1288, 712, 912, 720, 1424, 728,
			1936, 736, 1416, 744, 976, 752, 1488, 760, 2000, 768, 1536, 776,
			1552, 784, 1048, 792, 1560, 800, 1664, 808, 1680, 816, 1112, 824,
			1624, 832, 1792, 840, 1808, 848, 1176, 856, 1688, 864, 1920, 872,
			1936, 880, 1240, 888, 1752, 896, 1544, 904, 1560, 912, 1304, 920,
			1816, 928, 1672, 936, 1688, 944, 1368, 952, 1880, 960, 1800, 968,
			1816, 976, 1432, 984, 1944, 992, 1928, 1000, 1944, 1008, 1496, 1016,
			2008, 1032, 1152, 1040, 1056, 1048, 1568, 1064, 1408, 1072, 1120,
			1080, 1632, 1088, 1536, 1096, 1160, 1104, 1184, 1112, 1696, 1120,
			1552, 1128, 1416, 1136, 1248, 1144, 1760, 1160, 1664, 1168, 1312,
			1176, 1824, 1184, 1544, 1192, 1920, 1200, 1376, 1208, 1888, 1216,
			1568, 1224, 1672, 1232, 1440, 1240, 1952, 1248, 1560, 1256, 1928,
			1264, 1504, 1272, 2016, 1288, 1312, 1296, 1408, 1304, 1576, 1320,
			1424, 1328, 1416, 1336, 1640, 1344, 1792, 1352, 1824, 1360, 1920,
			1368, 1704, 1376, 1800, 1384, 1432, 1392, 1928, 1400, 1768, 1416,
			1680, 1432, 1832, 1440, 1576, 1448, 1936, 1456, 1832, 1464, 1896,
			1472, 1808, 1480, 1688, 1488, 1936, 1496, 1960, 1504, 1816, 1512,
			1944, 1520, 1944, 1528, 2024, 1560, 1584, 1592, 1648, 1600, 1792,
			1608, 1920, 1616, 1800, 1624, 1712, 1632, 1808, 1640, 1936, 1648,
			1816, 1656, 1776, 1672, 1696, 1688, 1840, 1704, 1952, 1712, 1928,
			1720, 1904, 1728, 1824, 1736, 1952, 1744, 1832, 1752, 1968, 1760,
			1840, 1768, 1960, 1776, 1944, 1784, 2032, 1864, 1872, 1848, 1944,
			1872, 1888, 1880, 1904, 1888, 1984, 1896, 2000, 1912, 2032, 1904,
			2016, 1976, 2032, 1960, 1968, 2008, 2032, 1992, 2016, 2024, 2032 };
	fft.fftLen = FFT_SIZE;
	fft.pTwiddle = twiddleCoef;
	fft.pBitRevTable = revIndexTable;
	fft.bitRevLength = REV_INDEX_TABLE_LENGTH;

	// assign the process and playback pointers
	processPtr = aBuf;
	playbackPtr = bBuf;
}

void FFTPhaser::process(float input) {
	// 1. check if it's time to perform the next FFT
	if (bufferIdx >= FFT_SIZE) {
		bufferFull();
		bufferIdx = 0;
	} else {
		++bufferIdx;
	}
	// 2. add the input into the process buffer
	processPtr[bufferIdx * 2] = input;
	// zero the imaginary value as well
	processPtr[bufferIdx * 2 + 1] = 0.0f;
	return playbackPtr[bufferIdx * 2];
}

// this guy does the heavy lifing really
void FFTPhaser::bufferFull() {
// Do a forward FFT on the newest buffer of audio data
arm_cfft_f32(&fft, processPtr, 0, 1);
// Do something interesting to the imaginary parts here
performEffect(processPtr);
// Do a reverse FFT to get back to the time domain
arm_cfft_f32(&fft, processPtr, 1, 1);
// swap the process and playback pointers
float* prevProcess = processPtr;
processPtr = playbackPtr;
playbackPtr = prevProcess;

}

void FFTPhaser::performEffect(float* buf){
	//TODO
}

