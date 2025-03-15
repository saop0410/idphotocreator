#include "main.h"

Mat img = imread("C:/imageArchive/ref1.bmp", 1);
Mat face_detected_img;
Mat small_rect_img;

float* magnitude_img = (float*)calloc(img.rows * img.cols, sizeof(float));

void main() {

    CascadeClassifier cascade;
    cascade.load("C:/opencv/sources/data/lbpcascades/lbpcascade_frontalface.xml");

     face_detected_img = img.clone();
     small_rect_img = img.clone();

    cascade.detectMultiScale(face_detected_img, faces, 1.1, 4, 0 | CV_HAAR_SCALE_IMAGE, Size(10, 10));

    Edge_Analysis(magnitude_img);

    
    for (int y = 0; y < faces.size(); y++)
    {
        Point lb1(faces[y].x + faces[y].width, faces[y].y + faces[y].height);
        Point tr1(faces[y].x, faces[y].y);
        rectangle(face_detected_img, lb1, tr1, Scalar(0, 255, 0), 3, 8, 0);

    
        /*Point lb3(faces[y].x + faces[y].width * 0.65, faces[y].y + faces[y].height *1.1);
        Point tr3(faces[y].x + faces[y].width * 0.35, faces[y].y);
        rectangle(small_rect_img, lb3, tr3, Scalar(0, 0, 255), 3, 8, 0);*/

       // 31~34 : 몸 쪽에 직선 masking

    } //input image cascade로 rect 있는 Mat face_detected_img 생성 && 작은 rect를 가진 small_rect_img 생성

    namedWindow("Face", WINDOW_AUTOSIZE);
    setMouseCallback("Face", onMouse, nullptr);

    imshow("Face", face_detected_img);
    
    // GUI를 유지
    waitKey(0);

}
