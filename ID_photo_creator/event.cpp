#include "main.h"

vector<Rect> faces;
int faceIndex;
Mat Grabcut_result;

void actionForFace(int faceIndex) {
    Mat input_cropped(faces[faceIndex].height * 2, faces[faceIndex].width * 2, CV_8UC3);
    Mat input_cropped_mask(faces[faceIndex].height * 2, faces[faceIndex].width * 2, CV_8UC3);
    
    int idx = 0, idy = 0;
    int choice = 0;

    imshow("ff", small_rect_img);


    for (int yy = faces[faceIndex].y - faces[faceIndex].height / 2; yy < faces[faceIndex].y + faces[faceIndex].height * 3 / 2; yy++) {
        for (int xx = faces[faceIndex].x - faces[faceIndex].width / 2; xx < faces[faceIndex].x + faces[faceIndex].width * 3 / 2; xx++) {
            input_cropped.at<Vec3b>(idy, idx)[0] = img.at<Vec3b>(yy, xx)[0];
            input_cropped.at<Vec3b>(idy, idx)[1] = img.at<Vec3b>(yy, xx)[1];
            input_cropped.at<Vec3b>(idy, idx)[2] = img.at<Vec3b>(yy, xx)[2];

            input_cropped_mask.at<Vec3b>(idy, idx)[0] = small_rect_img.at<Vec3b>(yy, xx)[0];
            input_cropped_mask.at<Vec3b>(idy, idx)[1] = small_rect_img.at<Vec3b>(yy, xx)[1];
            input_cropped_mask.at<Vec3b>(idy, idx)[2] = small_rect_img.at<Vec3b>(yy, xx)[2];

            idx++;
        }
        idy++;
        idx = 0;
    }

    Grabcut_result = GraphCut_mask(input_cropped, input_cropped_mask);
    imshow("Grabcut Image", Grabcut_result);
    waitKey(100); // 사용자가 확인할 때까지 대기

    menu(Grabcut_result);


}

void onMouse(int event, int x, int y, int flags, void* userdata) {
    int min = 0;
    if (event == EVENT_LBUTTONDOWN) { // 마우스 왼쪽 버튼 클릭 시
        for (int i = 0; i < faces.size(); i++) {
            if (faces[i].contains(Point(x, y))) { // 클릭한 좌표가 얼굴 영역 안에 있으면
                cout << "Face " << i + 1 << " clicked!" << endl;
                //여기다가 small rect 그리기

                circle(small_rect_img, Point(faces[i].x + faces[i].width / 2, faces[i].y + faces[i].height / 2), faces[i].width / 2 * 0.8, cv::Scalar(0, 0, 255), -1);
                Point lb4(faces[i].x + faces[i].width, faces[i].y + faces[i].height * 3 / 2 - 5);
                Point tr4(faces[i].x, faces[i].y + faces[i].height * 3 / 2 - 5);

                line(small_rect_img, lb4, tr4, Scalar(0, 0, 255), 5, 8, 0);



                min = HairEdge(i, magnitude_img);
                
                Point lb3(faces[i].x + faces[i].width * 0.55, faces[i].y + faces[i].height * 1.1);
                Point tr3(faces[i].x + faces[i].width * 0.45, min);
                rectangle(small_rect_img, lb3, tr3, Scalar(0, 0, 255), 3, 8, 0);

                Point lb10(faces[i].x + faces[i].width * 0.35, min);
                Point tr10(faces[i].x + faces[i].width * 0.65, min);
                line(small_rect_img, lb10, tr10, Scalar(0, 0, 255), 5, 8, 0);

                Point lb5(faces[i].x - faces[i].width / 2, faces[i].y - faces[i].height / 2);
                Point tr5(faces[i].x + faces[i].width * 3 / 2, faces[i].y - faces[i].height / 2);

                line(small_rect_img, lb5, tr5, Scalar(255, 0, 0), 7, 8, 0); //


                Point lb6(faces[i].x + faces[i].width * 3 / 2 , faces[i].y - faces[i].height / 2);
                Point tr6(faces[i].x + faces[i].width * 3 / 2, faces[i].y + faces[i].height);

                line(small_rect_img, lb6, tr6, Scalar(255, 0, 0), 7, 8, 0); // 

                Point lb7(faces[i].x - faces[i].width / 2, faces[i].y - faces[i].height / 2);
                Point tr7(faces[i].x - faces[i].width / 2, faces[i].y + faces[i].height);

                line(small_rect_img, lb7, tr7, Scalar(255, 0, 0), 7, 8, 0); // 경계 파랑


                actionForFace(i); // 클릭된 얼굴에 해당하는 작업 수행
                faceIndex = i;
                break;
            }
        }
    }
}
