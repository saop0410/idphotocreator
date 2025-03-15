#include "main.h"

#define BLK 16

int fx[9] = { -1,0,1,-1,0,1,-1,0,1 };
int fy[9] = { -1,-1,-1,0,0,0,1,1,1 };

void Edge_Analysis(float* magnitude) {
	Mat imgGray;
	cvtColor(img, imgGray, COLOR_BGR2GRAY);
	int height = imgGray.rows;
	int width = imgGray.cols;
	int cnt; int max = 0; int min = 1000;
	

	int conv_x, conv_y;

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {

			conv_x = 0;
			conv_y = 0;
			cnt = 0;

			for (int z = y - 1; z <= y + 1; z++) {
				for (int d = x - 1; d <= x + 1; d++) {
					if (z >= 0 && z < height && d >= 0 && d < width) {
						conv_x += imgGray.at<uchar>(z, d) * fx[cnt];
						conv_y += imgGray.at<uchar>(z, d) * fy[cnt];
					}
					cnt++;
				}
			}

			magnitude[y * width + x] = sqrt(conv_x * conv_x + conv_y * conv_y);
		}
	}

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			if (max < magnitude[i * width + j]) {
				max = magnitude[i * width + j];
			}

			if (min > magnitude[i * width + j]) {
				min = magnitude[i * width + j];
			}
		}
	}

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			magnitude[i * width + j] = 255 * (magnitude[i * width + j] - min) / (max - min);

		}
	}


}

int HairEdge(int faceIndex, float* magnitude) {
	int keep_go_up = 0;
	int height = img.rows;
	int width = img.cols;
	int edge_index;
	int yy = faces[faceIndex].y;
	int xx = faces[faceIndex].x + faces[faceIndex].width / 2;


	while (keep_go_up < 1 && yy > 1) {
		if (magnitude[yy * width + xx] > 30) // ¼öÄ¡ ¿Ã¸®¸é ¿Ã¸±¼ö·Ï »¡¸® ¸ØÃá´Ù.
			keep_go_up++;
		
		yy--;
	}
	edge_index = yy - 3;

	return edge_index;
}