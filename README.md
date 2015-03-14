``` imported from google code ```

This is a project on Optical Flow computation according to the Horn–Schunck method with additional result-improving techniques such as:

* Gaussian Pyramids
* Median Filter
* Penalty Functions
* Texture Decomposition
* Cubic Interpolation

##Main goal
The main goal of this project is to implement modern real-time optical flow algorithm in C++ using [OpenCV](http://opencv.willowgarage.com/wiki/) libraries.

##Project mile stones
Current Version: 1.0

| Feature |	Status |
| ------- | ------ |
| Basic Structure |	Done |
| Cubic interpolation |	Done |
| Multi grid linear solver |	In progress |
| Texture decomposition |	Done |
| Median filter |	Done |
| Error evaluation | Done |
| Optimization | Done |
| GUI |	Done |

###Compilation
In order to run/compile the code you will need OpenCV [installed and compiled](http://opencv.willowgarage.com/wiki/InstallGuide) in your PC.

##Examples
See Wiki UsageExample

###References
1. T. Brox, A. Bruhn, N. Papenberg, and J. Weickert. High accuracy optical flow estimation based on a theory for warping. ECCV, 2004.

2. D. Sun, S Roth and M. Black. Secrets of Optical Flow Estimation and Their Principles, CVPR 2010 (classic NL)

3. http://vision.middlebury.edu/flow/

#Old Wiki

## COOrdSparseMat

COOrdSparseMat is an implemetation of the [http://en.wikipedia.org/wiki/Sparse_matrix#Coordinate_list_.28COO.29 COO Sparse Matrix].
The advantages of the COO (Coordinate list) is fast insertion time, but the access time is O(non zero) which can be huge, thus we use the COOrdSparseMat to build a CRSSparseMat using the `CRSSparseMat(COOrdSparseMat&)` Constructor.


### Implementation Details
#### Constructor
```c++
COOrdSparseMat(int M, int N, int nz, float* val, int* row, int* col)
```
 * M - number of rows.
 * N - number of columns.
 * nz - number of estimated non zeros (better be higher then lower than the actual value).
 * val - an array of floats, will hold the values of the cells
 * row - an array of ints, the i-th cell will hold the row index of the i-th value.
 * col - an array of ints, the i-th cell will hold the column index of the i-th value.
 
#### Dimension functions
```c++
int rows();
```
Number of rows.
```c++
int cols();
```
Number of columns.
```c++
int nonZeros();
```
Number of non zeros in the sparse matrix _(Please notice: the COOrdSparseMat & CRSSparseMat classes dont guarantee that the internal nonZeros() value is correct)_.


#### Access Values Functions
```c++
float operator()(int i, int j);
```
Gets the value in the i-th row and the j-th column in the matrix. (works at O(non zeros), not recommended)

#### Set Values Functions
```c++
void set(int i, int j, float val);
```
Sets the value of an already existing non-zero value in the sparse matrix, *it will not add a new non-zero value to the matrix*.

## TheoryPseudoCode
Theory and Pseudo Code, explaining the internal operation of the alogrithm.

## Pseudo Code
This is the high-level operation of the algorithm.
```
calculate(Im1, Im2)
  Texture_Decomposition(Im1, Im2)
  flow = Create_flow()
  PyrIm1 = Gaussian_Pyramid(Im1)
  PyrIm2 = Gaussian_Pyramid(Im2)
  foreach level in PyrIm1 do
    Reshape_Flow(flow)
    Ix = X_Derivative(levelOfIm1)
    Iy = Y_Derivative(levelOfIm1)
    warp = Interpolate_Image(levelOfIm2, flow)
    It = warp - levelOfIm1
    [Operator, B] = Build_Flow_Operator(flow, Du, Dv, Ix, Iy, It)
    X = Solve(Operator, B)
    flow += Parse_Delta_Flow(X)
    flow = Median_Filter(flow)
```


## OpticalFlow
The main class in the project, calculates the optical flow via the static function `calculate`.
To learn more about the internal operation of the algorithm, see TheoryPseudoCode page in the Wiki.

### Implementation Details

```c++
static flowUV* OpticalFlow::calculate (const cv::Mat& Im1, const cv::Mat& Im2, const OpticalFlowParams& params, flowUV* oldFlow = NULL);
```
Caluclates the optical flow between 2 images with respect of the OpticalFlowParams instance.
 * Im1 - The first image (of type CV_32F and in grayscale color space).
 * Im2 - The second image (of type CV_32F and in grayscale color space).
 * params - an instance of OpticalFlowParams (see Wiki entry).
 * oldFlow - an initial guess (or old flow) for the flow, meant for usage when calculating flow of an image sequence or video under the assumption that the flow wont change drastically between consecutive frames in a high frame rate video.

 * return - a flowUV object with the result flow, needs to be deleted manually.
 
 
## PenaltyFunctionCompute
PenaltyFunctionCompute is a bundle of 6 different functions that restrict the current flow values to achieve improved results.

### Implementation Details
```c++
static void compute(cv::Mat & mat, PenaltyFunctionCompute::RobustFunctionType type, double sigma = 1, PenaltyFunctionCompute::Derivative derivative = Value, const double aSigma = 0.45);
```
 * mat - the cv::Mat matrix to alter.
 * type - which penalty (AKA Robust) function should be used.
 * sigma - used for the inner calculation of the penalty function, default value = 1.
 * derivative - which derivative of the chosen penalty function should be used when calculating, default value = Value.
 * aSigma - used for the inner calculation of the GeneralizedCharbonnier penalty function, default value = 0.45.

### enum RobustFunctionType
```c++
	enum RobustFunctionType
	{
		Charbonnier,
		GeneralizedCharbonnier,
		Gaussian,
		Geman_mcclure,
		Lorentzian,
		Quadratic
	};
```
 * Charbonnier
 * Generalized Charbonnier
 * Gaussian
 * Geman Mcclure
 * Lorentzian
 * Quadratic

### enum Derivative
```c++
	enum Derivative
	{
		Value,
		First,
		Second
	};
```
 * Value - Function value, no derivative.
 * First - First derivative.
 * Second - Second derivative.
 
 
 
## OpticalFlowParams

OpticalFlowParams is used to transfer parameters into `OpticalFlow::calculate` function in the OpticalFlow class.

### Implementation Details
```c++
	OpticalFlowParams(	const float alpha = 3.0f, 
				const int pyramidLevels = 5, 
				const float pyramidSpacing = 0.5f,
				const float weightedDeriveFactor = 0.5f,
				const bool displayDerivativs = false,
				const int medianFilterRadius = 5,
				const int sorIters = 100,
				const float overRelaxation = 1.9f,
				const bool checkResidualTolerance = true,
				const float residualTolerance = 0.01f,
				const int iters = 3, 
				const int linearIters = 1,
				const double penaltySigma = 0.01f,
				const PenaltyFunctionCompute::RobustFunctionType pfSpatialX = PenaltyFunctionCompute::Quadratic,
				const PenaltyFunctionCompute::Derivative pfSpatialXDeriv = PenaltyFunctionCompute::Second,
				const PenaltyFunctionCompute::RobustFunctionType pfSpatialY = PenaltyFunctionCompute::Quadratic,
				const PenaltyFunctionCompute::Derivative pfSpatialYDeriv = PenaltyFunctionCompute::Second,
				const PenaltyFunctionCompute::RobustFunctionType pdData = PenaltyFunctionCompute::Quadratic,
				const PenaltyFunctionCompute::Derivative pdDataDeriv = PenaltyFunctionCompute::Second,
				const bool useTextureDecomposition = true,
				const float TDtheta = 1.0f/8.0f,
				const int TDnIters = 100,
				const float TDalp = 0.95f,
				const bool displayTextureDecompositionOutput = false,
				const bool display = true)
```

 * alpha - smoothness weight in the flow operator.
 * pyramidLevels - the number of levels in the gaussian pyramid.
 * pyramidSpacing - the spacing factor between each level in the gaussian pyramis.
 * weightedDeriveFactor - weighting factor between the versions of the derivative (numerical from images and from interpolated image according to the current calculated flow).
 * displayDerivativs - display the derivatives.
 * medianFilterRadius - the radius of the median filter.
 * sorIters - number of iterations for the SOR run.
 * overRelaxation - over relaxation factor for the SOR run.
 * checkResidualTolerance - set whether to check if Ax is withing tolerance value from B in the SOR run.
 * residualTolerance - tolerance value for the SOR run.
 * iters - number of iterations per pyramid level.
 * linearIters - number of linearization iterations.
 * penaltySigma - sigma.
 * pfSpatialX - penalty for the X direction.
 * pfSpatialXDeriv - derivative for the X direction.
 * pfSpatialY - penalty for the Y direction.
 * pfSpatialYDeriv - derivative for the Y direction.
 * pdData - penalty for the data term.
 * pdDataDeriv - derivative for the data term.
 * useTextureDecomposition - use texture decomposition.
 * TDtheta - theta for the texture decomposition.
 * TDnIters - number of iterations for the texture decomposition.
 * TDalp - alpha for the texture decomposition.
 * displayTextureDecompositionOutput - display the return value of the texture decomposition run.
 * display - display the flow for each iteration.
 
 
 
# Usage Example

The source code seen in this page is included in the svn as `image_tester.cpp`
```c++
	#define OPTFLOW_TYPE			CV_32F
```
```
	cv::Mat tU;
	cv::Mat tV;
	UtilsFlow::ReadFlowFile("flow10.flo", tU, tV);
	UtilsFlow::DrawFlow(tU, tV, "Ground Truth");
	flowUV GT(tU, tV);
	
	cv::Mat image1 = cv::imread("frame10.png");
	cv::Mat image2 = cv::imread("frame11.png");
	
	cv::Mat image1Gray(image1.rows, image1.cols, CV_MAKETYPE(image1.depth(), 1));
	cv::Mat image2Gray(image2.rows, image2.cols, CV_MAKETYPE(image2.depth(), 1));
	cv::cvtColor(image1, image1Gray, CV_BGR2GRAY);
	cv::cvtColor(image2, image2Gray, CV_BGR2GRAY);
	
	image1.release();
	image2.release();

	cv::Mat image1GrayFloat(image1.rows, image1Gray.cols, OPTFLOW_TYPE);
	cv::Mat image2GrayFloat(image2.rows, image2Gray.cols, OPTFLOW_TYPE);
	image1Gray.convertTo(image1GrayFloat, OPTFLOW_TYPE);
	image2Gray.convertTo(image2GrayFloat, OPTFLOW_TYPE);

	image1Gray.release();
	image2Gray.release();

	OpticalFlowParams params(3, 5, 0.5f, 0.5f, false, 5, 100, 1.9f, false, 0.01f, 3, 1, 0.001f, 
									PenaltyFunctionCompute::Quadratic, 
									PenaltyFunctionCompute::Second, 
									PenaltyFunctionCompute::Quadratic,
									PenaltyFunctionCompute::Second,
									PenaltyFunctionCompute::Quadratic, 
									PenaltyFunctionCompute::Second,
									true, 1.0f/8.0f, 100, 0.95f, false, true);

	cout << "Image tester: calculating flow..." << endl;
		
	flowUV* UV = OpticalFlow::calculate(image1GrayFloat, image2GrayFloat, params);
	cout << "Image tester: ENDED calculating flow in " << endl;

	image1GrayFloat.release();
	image2GrayFloat.release();

	cout << "Image tester: drawing flow..." << endl;
	UtilsFlow::DrawFlow(UV->getU(), UV->getV(), "Flow");
	cout << "Image tester: calculating error..." << endl;
	float* err = FlowError::calcError(*UV, GT, verbose);
	cout << "Image tester: AAE " << err[0] << " STD " << err[1] << " average end point error " << err[2] << endl;

	delete UV;
	cout << "End." << endl;
	cv::waitKey(0);
```

### Explanation
```c++
        cv::Mat tU;
        cv::Mat tV;
        UtilsFlow::ReadFlowFile("flow10.flo", tU, tV);
        UtilsFlow::DrawFlow(tU, tV, "Ground Truth");
        flowUV GT(tU, tV);
```
Loading and drawing the Ground Truth file for error calculations (see [http://vision.middlebury.edu/flow/data/ Middlebury]) and load it into a flowUV object. 

```c++
	cv::Mat image1 = cv::imread("frame10.png");
	cv::Mat image2 = cv::imread("frame11.png");
	
	cv::Mat image1Gray(image1.rows, image1.cols, CV_MAKETYPE(image1.depth(), 1));
	cv::Mat image2Gray(image2.rows, image2.cols, CV_MAKETYPE(image2.depth(), 1));
	cv::cvtColor(image1, image1Gray, CV_BGR2GRAY);
	cv::cvtColor(image2, image2Gray, CV_BGR2GRAY);
	
	image1.release();
	image2.release();

	cv::Mat image1GrayFloat(image1.rows, image1Gray.cols, OPTFLOW_TYPE);
	cv::Mat image2GrayFloat(image2.rows, image2Gray.cols, OPTFLOW_TYPE);
	image1Gray.convertTo(image1GrayFloat, OPTFLOW_TYPE);
	image2Gray.convertTo(image2GrayFloat, OPTFLOW_TYPE);

	image1Gray.release();
	image2Gray.release();
```
Loads the 2 images, converts them to grayscale and then convert the cv::Mat to CV_32F (a matrix of floats).

```c++
	OpticalFlowParams params(3, 5, 0.5f, 0.5f, false, 5, 100, 1.9f, false, 0.01f, 3, 1, 0.001f, 
									PenaltyFunctionCompute::Quadratic, 
									PenaltyFunctionCompute::Second, 
									PenaltyFunctionCompute::Quadratic,
									PenaltyFunctionCompute::Second,
									PenaltyFunctionCompute::Quadratic, 
									PenaltyFunctionCompute::Second,
									true, 1.0f/8.0f, 100, 0.95f, false, true);
```
Creates the parameters for the optical flow calculation - see Wiki entry for more info about the OpticalFlowParams class.

```c++
	flowUV* UV = OpticalFlow::calculate(image1GrayFloat, image2GrayFloat, params);
```
Calculate the optical flow.

```c++
	cout << "Image tester: drawing flow..." << endl;
	UtilsFlow::DrawFlow(UV->getU(), UV->getV(), "Flow");
	cout << "Image tester: calculating error..." << endl;
	float* err = FlowError::calcError(*UV, GT, verbose);
	cout << "Image tester: AAE " << err[0] << " STD " << err[1] << " average end point error " << err[2] << endl;
```
Drawing output flow and calculating error values,
 * AAE - Average Angular Error
 * STD - Standard Deviation
 
 
## LinearSolver
Linear Solver for equations of type Ax = B using SOR method for CRS sparse matrices.
	
### Implementation Details
	

The LinearSolver static class implements 2 version of the SOR method
```c++
LinearSolver::sparseMatSorNoResidual(CRSSparseMat& A, FArray& X0, FArray& X, FArray& B, const float w, const int iters)
```
This implementation provides the simplest method to solve using the SOR method, provide a the left side CRS sparse matrix, an initial solution X0, a destenation X, the right side B, the over relaxation factor w and the total number of iterations.
As implied by the function name, this implementation of the SOR method does not use any residual calculation to stop its operation when the answer Ax is withing a tolerance value from B.
```c++
LinearSolver::sparseMatSor(CRSSparseMat& A, FArray& X0, FArray& X, FArray& B, const float w, const int iters, const float tolerance)
```
In addition to what specified for the previous implementation of the SOR method, the second implementation provides a method to stop the calculation when Ax is within a tolerance value from B, this tolerance value is specified by the last float parameter of the function.
_*Please notice, the tolerance is checked every 5 iterations of the SOR, this can be bad for performance if the number of iteration is set to a high value.* (if you're using a high value, it might be better to use the first, residual-less implementation)

## CRSSparseMat

CRSSparseMat is an implemetation of the [CRS Sparse Matrix](http://en.wikipedia.org/wiki/Sparse_matrix#Compressed_sparse_row_.28CSR_or_CRS.29).
The advantages of the CRS (Compressed sparse row) are fast cell access (with the correct implementation can be reduced to O(1)) but on the other hand, building such sparse matrix is rather slow, the solution is explained in the `build` function and the `CRSSparseMat(COOrdSparseMat&)` Constructor.


### Implementation Details
#### Constructor
```c++
CRSSparseMat()
```
Default constructor.

### Initialization functions
```c++
void build(const COOrdSparseMat&, int nz);
void build(const COOrdSparseMat&);
```
Both functions provide a method for building a CRS sparse matrix out of a COO sparse matrix (The COO form is the simplest form of sparse matrix, and is as fast as it gets regarding to insertion of new values).
 * nz - the number of non zero values in the COOrdSparseMat.
 * the function `void build(const COOrdSparseMat&);` calls `void build(const COOrdSparseMat&, int nz);` with the COOrdSparseMat's 'nonZeros()' value _(Please notice: the COOrdSparseMat & CRSSparseMat classes dont guarantee that the internal nonZeros() value is correct)_.

### Dimension functions
```c++
int dimR();
```
Number of rows.
```c++
int dimC();
```
Number of columns.
```c++
int nonZeros();
```
Number of non zeros in the sparse matrix _(Please notice: the COOrdSparseMat & CRSSparseMat classes dont guarantee that the internal nonZeros() value is correct)_.

### Arithmetic Operations
```c++
void MulVector(const float* x, float* b);
```
Multiply the Matrix by an array x and put the answer in b.
```c++
void MulScalar(const float x);
```
Multiply the Matrix by a float scalar x.

### Access Values Functions
```c++
float operator()(int i, int j);
```
Gets the value in the i-th row and the j-th column in the matrix.
This is the simplest method to get a value out of the matrix, but it is not very efficient for sequential read of the matrix. _(For sequential read of the matrix use the next 3 functions)_
```c++
float& val(int i);
```
Gets the i-th value out of the values array.
```c++
int& rowPtr(int i);
```
Gets the index to the values and columns arrays in which the i-th row starts at.
```c++
int& colIdx(int i);
```
Gets the i-th value out of the column array.
