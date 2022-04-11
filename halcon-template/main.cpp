#include "pch.h"
#include "preprocess.h"

namespace plt = matplotlibcpp;

using namespace std::chrono;
using namespace std;
using namespace HalconCpp;

// Local iconic variables
HObject  ho_Image, ho_ImagePart, ho_ImagePartGray;
HObject  ho_ImageBinomial, ho_EdgeAmplitudeTenengrad;

// Local control variables
HTuple  hv_LROW, hv_LColumn, hv_RROW, hv_RColumn;
HTuple  hv_NumImages, hv_I, hv_ValueTenengrad, hv_DeviationTenengrad;


void test_rgb2graywithfilter()
{
	vector<float> time_vec;
	hv_NumImages = 149;
	HTuple end_val7 = hv_NumImages;
	HTuple step_val7 = 1;

	for (hv_I = 0; hv_I.Continue(end_val7, step_val7); hv_I += step_val7)
	{
		ReadImage(&ho_Image, ("E:/MyData/2022-01-21/乳腺纤维瘤/" + hv_I) + ".bmp");

		auto start = std::chrono::steady_clock::now();
		Rgb1ToGray(ho_Image, &ho_ImagePartGray);
		MedianImage(ho_ImagePartGray, &ho_ImageBinomial, "circle", 3, "continued");
		auto end = std::chrono::steady_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
		time_vec.push_back(duration.count());
	}

	float avgtime = accumulate(begin(time_vec), end(time_vec), 0.0) / time_vec.size();
	
	double variance = 0.0;
	for (int i = 0; i < time_vec.size(); i++)  { variance += pow(time_vec[i] - avgtime, 2); }
	variance = variance / time_vec.size();
	
	double standard_deviation = sqrt(variance);
	double maxtime = *max_element(time_vec.begin(), time_vec.end());

	cout << "avgtime: " << avgtime << "ms" << '\n';
	cout << "size: " << time_vec.size() << '\n';
	cout << "max_time: " << maxtime << "ms" << '\n';
	cout << "standard_deviation: " << standard_deviation << '\n';

	/*plt::plot(time_vec, "bo-");
	plt::title("halcon_time");
	plt::show();*/
}

void total_Telegrad()
{
	vector<double> res;
	vector<float> time_vec;

	hv_LROW = 300;
	hv_LColumn = 320;
	hv_RROW = 899;
	hv_RColumn = 1279;
	hv_NumImages = 149;

	HTuple end_val7 = hv_NumImages;
	HTuple step_val7 = 1;

	for (hv_I = 0; hv_I.Continue(end_val7, step_val7); hv_I += step_val7)
	{
		//ReadImage(&ho_Image, ("E:/MyData/2022-01-21/骨细胞切片/" + hv_I) + ".bmp");
		//ReadImage(&ho_Image, ("E:/MyData/2022-01-21/卵巢囊性纤维瘤/" + hv_I) + ".bmp");
		ReadImage(&ho_Image, ("E:/MyData/2022-01-21/乳腺纤维瘤/" + hv_I) + ".bmp");

		//CropRectangle1(ho_Image, &ho_ImagePart, hv_LROW, hv_LColumn, hv_RROW, hv_RColumn);

		auto start = std::chrono::steady_clock::now();
		Rgb1ToGray(ho_Image, &ho_ImagePartGray);
		MedianImage(ho_ImagePartGray, &ho_ImageBinomial, "circle", 3, "continued");
		SobelAmp(ho_ImageBinomial, &ho_EdgeAmplitudeTenengrad, "sum_sqrt", 3);
		Intensity(ho_EdgeAmplitudeTenengrad, ho_EdgeAmplitudeTenengrad, &hv_ValueTenengrad, &hv_DeviationTenengrad);

		auto end = std::chrono::steady_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
		time_vec.push_back(duration.count());

		double f = hv_ValueTenengrad;
		res.push_back(f);
	}

	float avgtime = accumulate(begin(time_vec), end(time_vec), 0.0) / time_vec.size();
	
	double variance = 0.0;
	for (int i = 0; i < time_vec.size(); i++)  { variance += pow(time_vec[i] - avgtime, 2); }
	variance = variance / time_vec.size();
	
	double standard_deviation = sqrt(variance);
	double maxtime = *max_element(time_vec.begin(), time_vec.end());

	cout << "avgtime: " << avgtime << "ms" << '\n';
	cout << "size: " << time_vec.size() << '\n';
	cout << "max_time: " << maxtime << "ms" << '\n';
	cout << "standard_deviation: " << standard_deviation << '\n';

	/*for (auto i : res)
		cout << i << ' ';
	cout << '\n';*/

	//格式转换
	/*int H = hv_Width[0].I();
	int W = hv_Height[0].I();
	cout << H << ' ' << W << endl;*/

	plt::plot(res);
	plt::title("f");
	plt::show();
}

void total_FC()
{
	vector<double> res;
	vector<float> time_vec;

	hv_LROW = 300;
	hv_LColumn = 320;
	hv_RROW = 899;
	hv_RColumn = 1279;
	hv_NumImages = 149;

	HTuple end_val7 = hv_NumImages;
	HTuple step_val7 = 1;

	for (hv_I = 0; hv_I.Continue(end_val7, step_val7); hv_I += step_val7)
	{
		//ReadImage(&ho_Image, ("E:/MyData/2022-01-21/骨细胞切片/" + hv_I) + ".bmp");
		ReadImage(&ho_Image, ("E:/MyData/2022-01-21/卵巢囊性纤维瘤/" + hv_I) + ".bmp");
		//ReadImage(&ho_Image, ("E:/MyData/2022-01-21/乳腺纤维瘤/" + hv_I) + ".bmp");

		//CropRectangle1(ho_Image, &ho_ImagePart, hv_LROW, hv_LColumn, hv_RROW, hv_RColumn);

		auto start = std::chrono::steady_clock::now();
		Rgb1ToGray(ho_Image, &ho_ImagePartGray);
		MedianImage(ho_ImagePartGray, &ho_ImageBinomial, "circle", 3, "continued");
		
		Intensity(ho_ImageBinomial, ho_ImageBinomial, &hv_ValueTenengrad, &hv_DeviationTenengrad);

		auto end = std::chrono::steady_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
		time_vec.push_back(duration.count());

		double f = hv_DeviationTenengrad;
		res.push_back(f);
	}

	float avgtime = accumulate(begin(time_vec), end(time_vec), 0.0) / time_vec.size();

	double variance = 0.0;
	for (int i = 0; i < time_vec.size(); i++) { variance += pow(time_vec[i] - avgtime, 2); }
	variance = variance / time_vec.size();

	double standard_deviation = sqrt(variance);
	double maxtime = *max_element(time_vec.begin(), time_vec.end());

	cout << "avgtime: " << avgtime << "ms" << '\n';
	cout << "size: " << time_vec.size() << '\n';
	cout << "max_time: " << maxtime << "ms" << '\n';
	cout << "standard_deviation: " << standard_deviation << '\n';

	/*for (auto i : res)
		cout << i << ' ';
	cout << '\n';*/

	//格式转换
	/*int H = hv_Width[0].I();
	int W = hv_Height[0].I();
	cout << H << ' ' << W << endl;*/

	/*plt::plot(res);
	plt::title("f");
	plt::show();*/
}

int main()
{
	auto start = std::chrono::steady_clock::now();
	//test_rgb2graywithfilter();
	//total_Telegrad();
	total_FC();
	auto end = std::chrono::steady_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	cout << "total time :" << duration.count() << "ms" << endl;
	//test_rgb2gray();
}