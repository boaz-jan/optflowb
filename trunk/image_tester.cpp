#include <iostream>
#include <stdio.h>
#include "highgui.h"
#include "GaussPyramid.h"
#include "coarse2FineCompute.h"
#include "SparseToolKit.h"
#include "optical_flow_demo.h"
#include <ctime>

using namespace std;

void test(){
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*double an[] ={0,0,0,0,0,0,0,0,0};
	double a[] = {1,2,3,4,5,6,7,8,9};
	double b[]={2,2,2,3,3,3,4,5,6};
	CvMat* matans = &cvMat( 3, 3, CV_64FC1, an ); // 64FC1 for double
	CvMat* matans2 = &cvMat( 3, 3, CV_64FC1, an ); // 64FC1 for double
	CvMat* mata = &cvMat( 3, 3, CV_64FC1, a );
	CvMat* matb = &cvMat( 3, 3, CV_64FC1, b );
	cvMul(mata,matb,matans);
	cvMul(matb,mata,matans2);
	PrintMat(mata);
	PrintMat(matb);
	PrintMat(matans);
	PrintMat(matans2);
	cout<<"a"<<endl;*/
	//GPyramid1.ConstructPyramid(*img,ratio,minWidth);

	
	//IplImage *imga1=cvCreateImage(cvSize(3,3), IPL_DEPTH_32F,1);
	IplImage *psidashFS1=cvCreateImage(cvSize(3,3), IPL_DEPTH_32F,1);
	IplImage *psidashFS2=cvCreateImage(cvSize(3,3), IPL_DEPTH_32F,1);
	IplImage *u=cvCreateImage(cvSize(3,3), IPL_DEPTH_32F,1);
	IplImage *pdfaltSumXX=cvCreateImage(cvSize(3,3), IPL_DEPTH_32F,1);
	/*((float*)imga1->imageData)[0]=1;
	((float*)imga1->imageData)[1]=2;
	((float*)imga1->imageData)[2]=3;
	((float*)imga1->imageData)[3]=4;
	((float*)imga1->imageData)[4]=5;
	((float*)imga1->imageData)[5]=6;
	((float*)imga1->imageData)[6]=7;
	((float*)imga1->imageData)[7]=8;
	((float*)imga1->imageData)[8]=9;

	((float*)u->imageData)[0]=1;
	((float*)u->imageData)[1]=1;
	((float*)u->imageData)[2]=1;
	((float*)u->imageData)[3]=2;
	((float*)u->imageData)[4]=2;
	((float*)u->imageData)[5]=2;
	((float*)u->imageData)[6]=3;
	((float*)u->imageData)[7]=3;
	((float*)u->imageData)[8]=3;

	((float*)psidashFS2->imageData)[0]=4;
	((float*)psidashFS2->imageData)[1]=5;
	((float*)psidashFS2->imageData)[2]=6;
	((float*)psidashFS2->imageData)[3]=4;
	((float*)psidashFS2->imageData)[4]=5;
	((float*)psidashFS2->imageData)[5]=6;
	((float*)psidashFS2->imageData)[6]=4;
	((float*)psidashFS2->imageData)[7]=5;
	((float*)psidashFS2->imageData)[8]=6;
	
	((float*)psidashFS1->imageData)[0]=1;
	((float*)psidashFS1->imageData)[1]=2;
	((float*)psidashFS1->imageData)[2]=3;
	((float*)psidashFS1->imageData)[3]=1;
	((float*)psidashFS1->imageData)[4]=2;
	((float*)psidashFS1->imageData)[5]=3;
	((float*)psidashFS1->imageData)[6]=1;
	((float*)psidashFS1->imageData)[7]=2;
	((float*)psidashFS1->imageData)[8]=3;*/
	//cout<<"FS1"<<endl;
	//toolsKit::IPL_print(psidashFS1);
	//cout<<"FS2"<<endl;
	//toolsKit::IPL_print(psidashFS2);
	// cvZero(pdfaltSumXX);


	double b[] = {1,-1};
	CvMat* matOneNegOne = &cvMat( 1, 2, CV_64FC1, b );
	cvFilter2D(psidashFS1,pdfaltSumXX,matOneNegOne);
	
	
	//cout<<"matOneNegOne"<<endl;
	//toolsKit::IPL_print(	matOneNegOne);
	cout<<"ux"<<endl;
	toolsKit::IPL_print(pdfaltSumXX);
	


 cvZero(pdfaltSumXX);


/*
	//init
	IplImage* tempLeft1=cvCreateImage(cvSize( psidashFS1->width, psidashFS1->height ),psidashFS1->depth,psidashFS1->nChannels);
	IplImage* tempLeft2=cvCreateImage(cvSize( psidashFS1->width, psidashFS1->height ),psidashFS1->depth,psidashFS1->nChannels);
	IplImage* tempLeft3=cvCreateImage(cvSize( psidashFS1->width, psidashFS1->height ),psidashFS1->depth,psidashFS1->nChannels);
	IplImage* tempLeft4=cvCreateImage(cvSize( psidashFS1->width, psidashFS1->height ),psidashFS1->depth,psidashFS1->nChannels);

	//psidashFS2(2:2:end ,  1:2:2*wt)* ( u(2:ht+1, 1:wt)  - u(2:ht+1, 2:wt+1) ) + //left ,ans2
	toolsKit::IPL_sub_left(u,u,tempLeft1);
	toolsKit::IPL_mul_left(tempLeft1,psidashFS2,tempLeft1);
	//psidashFS2(2:2:end ,  3:2:end) * ( u(2:ht+1, 3:end) - u(2:ht+1, 2:wt+1) ) + //right ,ans2
	toolsKit::IPL_sub_right(u,u,tempLeft2);
	toolsKit::IPL_mul_right(tempLeft2,psidashFS2,tempLeft2);
	//psidashFS1(1:2:2*ht,  2:2:end) * ( u(1:ht, 2:wt+1)  - u(2:ht+1, 2:wt+1) ) + //top ,ans1
	toolsKit::IPL_sub_top(u,u,tempLeft3);
	toolsKit::IPL_mul_top(tempLeft3,psidashFS1,tempLeft3);
	//psidashFS1(3:2:end ,  2:2:end) * ( u(3:end, 2:wt+1) - u(2:ht+1, 2:wt+1) )   //bottom ,ans1
	toolsKit::IPL_sub_bottom(u,u,tempLeft4);
	toolsKit::IPL_mul_bottom(tempLeft4,psidashFS1,tempLeft4);

	//temp1+temp2+temp3+temp4
	cvAdd(tempLeft1,tempLeft2,pdfaltSumXX);
	cvAdd(pdfaltSumXX,tempLeft3,pdfaltSumXX);
	cvAdd(pdfaltSumXX,tempLeft4,pdfaltSumXX);
	//- 1*pdfaltsumXX - needed for use in the next computation
	toolsKit::cvMulScalar(pdfaltSumXX,-1);
	cvReleaseImage(&tempLeft1);
	cvReleaseImage(&tempLeft2);
	cvReleaseImage(&tempLeft3);
	cvReleaseImage(&tempLeft4);	

	cout<<"pdfaltSumXX"<<endl;
	toolsKit::IPL_print(pdfaltSumXX);
	//toolsKit::IPL_print(imga1);

	*/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}




int main (int argc,char** argv) 
{ 
	double error_const=0.001;
	//IPL_DEPTH_32F IPL_DEPTH_8U
	coarse2FineCompute coarse2fComp(IPL_DEPTH_32F,error_const);
	double ratio=0.75;
	int minWidth=10;
	int outerIter=3;
	int innerIter=100;
	double alpha = 2 ; // Global smoothness variable.
	double gamma = 0 ; // Global weight for derivatives.


	std::clock_t start;
	double diff;

	IplImage* img1= cvLoadImage(argv[1],CV_LOAD_IMAGE_COLOR);//zero is for grayscale  CV_LOAD_IMAGE_GRAYSCALE
	IplImage* img2= cvLoadImage(argv[2],CV_LOAD_IMAGE_COLOR); //1 is for color CV_LOAD_IMAGE_COLOR


	IplImage *img1_32 = cvCreateImage(cvSize(img1->width, img1->height), coarse2fComp._imageDepth, img1->nChannels);
	IplImage *img2_32 = cvCreateImage(cvSize(img1->width, img1->height), coarse2fComp._imageDepth, img1->nChannels);

	cvZero(img1_32);
	cvZero(img2_32);
	//upscale from char to float
	cvConvertScale(img1, img1_32, 1.0/255);
	cvConvertScale(img2, img2_32, 1.0/255);


	//cvSmooth(img1_32,img1_32,CV_GAUSSIAN);
	//cvSmooth(img2_32,img2_32,CV_GAUSSIAN);

	IplImage *img1_32g = cvCreateImage(cvSize(img1->width, img1->height), coarse2fComp._imageDepth, 1);
	IplImage *img2_32g = cvCreateImage(cvSize(img1->width, img1->height), coarse2fComp._imageDepth, 1);

	cvCvtColor( img1_32, img1_32g, CV_BGR2GRAY );
	cvCvtColor( img2_32, img2_32g, CV_BGR2GRAY );


	//cvNormalize(img1_32g,img1_32g,127,0,CV_MINMAX); //CV_MINMAX
	//cvNormalize(img2_32g,img2_32g,127,0,CV_MINMAX); 

	
	const IplImage *img1_33_file=NULL;
	const IplImage *img2_33_file=NULL;
	img1_33_file=toolsKit::IplFromFile("c:\\a\\urban1r.txt");
//	img1_33_file=toolsKit::IplFromFile("c:\\a\\1_15_15.txt");
//	img1_33_file=toolsKit::IplFromFile("c:\\a\\Urban3_1s.txt");	
	//img2_33_file=toolsKit::IplFromFile("c:\\a\\2_15_15.txt");
	img2_33_file=toolsKit::IplFromFile("c:\\a\\urban2r.txt");
//	img2_33_file=toolsKit::IplFromFile("c:\\a\\Urban3_2s.txt");
		
	toolsKit::cvShowManyImages("img1,img2 ",2,img1_32g,img2_32g);	
	cvWaitKey(1);
	
	start = std::clock();
	flowUV* UV=coarse2fComp.Coarse2FineFlow(img1_32g, img2_32g, 
											alpha,gamma,
											ratio,minWidth, 
											outerIter,innerIter);
	diff = ( std::clock() - start ) / (double)CLOCKS_PER_SEC;
	std::cout<<"BROX pyramid alg. took "<< diff <<" secs"<<endl;

	 cvReleaseImage(&img1_32g);
	 cvReleaseImage(&img2_32g);
	 cvReleaseImage(&img1_32);
	 cvReleaseImage(&img2_32);
	 cvReleaseImage(&img1);
	 cvReleaseImage(&img2);

	 toolsKit::drawFlow(UV->getU(),UV->getV(),0);


	cout<<"fin"<<endl;
} 


