#include "main.h"

int angle = 0;

Mat GraphCut_mask(Mat input, Mat input_with_mask)
{
    int x, y;
    int height, width;
    height = input.rows;
    width = input.cols;
    Rect rectangle(0, 0, width - 1, height - 1);
    Mat mask = Mat::ones(height, width, CV_8UC1) * GC_PR_BGD;
    // Mask generation : red  foreground / blue  background
    for (y = 0; y < height; y++) {
        for (x = 0; x < width; x++) {
            if (input_with_mask.at<Vec3b>(y, x)[0] == 255 && input_with_mask.at<Vec3b>(y, x)[1] == 0 &&
                input_with_mask.at<Vec3b>(y, x)[2] == 0) {
                mask.at<uchar>(y, x) = GC_BGD; //background
            }
            else if (input_with_mask.at<Vec3b>(y, x)[0] == 0 && input_with_mask.at<Vec3b>(y, x)[1] == 0 &&
                input_with_mask.at<Vec3b>(y, x)[2] == 255) {
                mask.at<uchar>(y, x) = GC_FGD; //foreground
            }
        }
    }
    Mat final_result = Mat::zeros(height, width, CV_8UC3);
    Mat mask_result = Mat::zeros(height, width, CV_8UC1);
    Mat bg, fg;
    final_result = input.clone();
    grabCut(input, mask, rectangle, bg, fg, 8, GC_INIT_WITH_MASK);
    for (y = 0; y < height; y++) {
        for (x = 0; x < width; x++) {
            if (mask.at<uchar>(y, x) == GC_FGD || mask.at<uchar>(y, x) == GC_PR_FGD) {
                mask_result.at<uchar>(y, x) = 255;
            }
            else {
                final_result.at<Vec3b>(y, x)[0] = 255;
                final_result.at<Vec3b>(y, x)[1] = 255;
                final_result.at<Vec3b>(y, x)[2] = 255;
            }
        }
    }

    return final_result;

}

Mat shiftImageDown(Mat Grabcut_result) {
    // 이미지를 아래로 한 픽셀 이동
    Mat shifted = Mat::zeros(Grabcut_result.size(), Grabcut_result.type());
    
    shifted.setTo(Scalar(Grabcut_result.at<Vec3b>(0, 0)[0], Grabcut_result.at<Vec3b>(0, 0)[1], Grabcut_result.at<Vec3b>(0, 0)[2]));
    Grabcut_result(Rect(0, 0, Grabcut_result.cols, Grabcut_result.rows - 1)).copyTo(shifted(Rect(0, 1, Grabcut_result.cols, Grabcut_result.rows - 1))); // 이미지 이동
    Grabcut_result = shifted;
    return Grabcut_result;
}

Mat MakeResultDown(Mat Grabcut_result) {
    int height, width;
    height = Grabcut_result.rows;
    width = Grabcut_result.cols;
   // int maximum = 1;
  
    for (int i = 0; i < 3; i++) {
        Grabcut_result = shiftImageDown(Grabcut_result);
    }

    /*while (maximum) {
        if (isBottomRowAllWhite(Grabcut_result))
             Grabcut_result = shiftImageDown(Grabcut_result);
        else maximum = 0;
    }*/


    return Grabcut_result;

}

Mat ChangeBackgroundColor(Mat Grabcut_result) {
    int B, G, R;
    printf("원하는 색상을 입력하세요 -> B: G: R:\n");
    scanf("%d\n%d\n%d", &B, &G, &R);
    int height = Grabcut_result.rows;
    int width = Grabcut_result.cols;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (Grabcut_result.at<Vec3b>(y, x)[0] == 255 && Grabcut_result.at<Vec3b>(y, x)[1] == 255 && Grabcut_result.at<Vec3b>(y, x)[2] == 255) {
                Grabcut_result.at<Vec3b>(y, x)[0] = B;
                Grabcut_result.at<Vec3b>(y, x)[1] = G;
                Grabcut_result.at<Vec3b>(y, x)[2] = R;
            }
        }
    }
    return Grabcut_result;
}

Mat ImgResize(Mat Grabcut_result) {
    int height, width;
    printf("원하는 크기를 입력하세요 -> Height: Width:");
    scanf("%d\n%d", &height, &width);
    resize(Grabcut_result, Grabcut_result, Size(width, height), INTER_LINEAR);
    return Grabcut_result;
    
}

Mat ImgRotate(Mat Grabcut_result) {
    int width, height, nwidth, nheight;
    int x, y, nx, ny, sx, sy, sx1, sy1, sx2, sy2;
    int rVal, gVal, bVal, avg;

    width = Grabcut_result.cols;
    height = Grabcut_result.rows;

    printf("angle? : ");
    scanf("%d", &angle);

    float rad = angle * PI / 180;

    nwidth = width * cos(rad) + height * sin(rad);
    nheight = width * sin(rad) + height * cos(rad);

    Mat output(height, width, CV_8UC3);

    int ori_bVal = Grabcut_result.at<Vec3b>(0, 0)[0];
    int ori_gVal = Grabcut_result.at<Vec3b>(0, 0)[1];
    int ori_rVal = Grabcut_result.at<Vec3b>(0, 0)[2];

    for (y = 0; y < height; y++) {
        for (x = 0; x < width; x++) {

            nx = x - width / 2;
            ny = y - height / 2;


            sy = -sin(rad) * nx + cos(rad) * ny;
            sx = cos(rad) * nx + sin(rad) * ny;

            sy += height / 2;
            sx += width / 2;

            if (sy < height && sx < width && sy >= 0 && sx >= 0) {

                sy1 = (int)sy;
                sx1 = (int)sx;

                sy2 = (int)(sy + 0.5);
                sx2 = (int)(sx + 0.5);

                bVal = (Grabcut_result.at<Vec3b>(sy1, sx1)[0] + Grabcut_result.at<Vec3b>(sy1, sx2)[0] + Grabcut_result.at<Vec3b>(sy2, sx1)[0] + Grabcut_result.at<Vec3b>(sy2, sx2)[0]) / 4;

                gVal = (Grabcut_result.at<Vec3b>(sy1, sx1)[1] + Grabcut_result.at<Vec3b>(sy1, sx2)[1] + Grabcut_result.at<Vec3b>(sy2, sx1)[1] + Grabcut_result.at<Vec3b>(sy2, sx2)[1]) / 4;

                rVal = (Grabcut_result.at<Vec3b>(sy1, sx1)[2] + Grabcut_result.at<Vec3b>(sy1, sx2)[2] + Grabcut_result.at<Vec3b>(sy2, sx1)[2] + Grabcut_result.at<Vec3b>(sy2, sx2)[2]) / 4;

                output.at<Vec3b>(y, x)[0] = bVal;
                output.at<Vec3b>(y, x)[1] = gVal;
                output.at<Vec3b>(y, x)[2] = rVal;
            }
            else {
                output.at<Vec3b>(y, x)[0] = ori_bVal;
                output.at<Vec3b>(y, x)[1] = ori_gVal;
                output.at<Vec3b>(y, x)[2] = ori_rVal;
            }

        }
    }
    return output;
}

void menu(Mat Grabcut_result) {
    int choice;
    while (true) {

        printf("\n---------------------------------\n편집 메뉴: \n1. 배경 색 변경\n2. 이미지 크기 조정\n3. 이미지 회전\n4. 이미지 내리기\n5. 이미지 저장\n6. 종료\n---------------------------------\n");
        scanf("%d", &choice);

        if (choice == 6) {
            printf("프로그램을 종료합니다.");
            exit(0);
        }

        switch (choice) {
        case 1:
            Grabcut_result = ChangeBackgroundColor(Grabcut_result);
            imshow("Color Changing Image", Grabcut_result);
            waitKey(100);
            break;
        case 2:
            Grabcut_result = ImgResize(Grabcut_result);
            imshow("Resized Image", Grabcut_result);
            waitKey(100);
            break;
        case 3:
            Grabcut_result = ImgRotate(Grabcut_result);
            imshow("Rotated Image", Grabcut_result);
            waitKey(100);
            break;

        case 4:
            Grabcut_result = MakeResultDown(Grabcut_result);
            imshow("Image Down", Grabcut_result);
            waitKey(100);
            break;
   
        case 5:
            imwrite("C:/imageArchive/Your ID photo.bmp", Grabcut_result);
            printf("Image saved!!\n");
            break;          
        }
    }

}

//bool isBottomRowAllWhite(const Mat& Grabcut_result) {
//    int height = Grabcut_result.rows;
//    int width = Grabcut_result.cols;
//
//    // 가장 아래 행의 모든 픽셀 확인
//    for (int x = 0; x < width; x++) {
//        Vec3b pixel = Grabcut_result.at<Vec3b>(height - 1, x);
//        if (!(pixel[0] == 255 && pixel[1] == 255 && pixel[2] == 255)) {
//            // 흰색이 아닌 픽셀이 있으면 false 반환
//            return false;
//        }
//    }
//
//    // 모든 픽셀이 흰색이면 true 반환
//    return true;
//}