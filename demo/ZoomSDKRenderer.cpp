//GetVideoRawData

#include "rawdata/rawdata_video_source_helper_interface.h"
#include "ZoomSDKRenderer.h"
#include "zoom_sdk_def.h" 
#include <iostream>


#include <fstream>
#include <cstring>
#include <string>
#include <cstdlib>
#include <cstdint>
#include <cstdio>




void ZoomSDKRenderer::onRawDataFrameReceived(YUVRawDataI420* data)
{
	std::cout << "onRawDataFrameReceived." << std::endl;

	std::cout << "width." << data->GetStreamWidth() << std::endl;
	std::cout << "height." << data->GetStreamHeight() << std::endl;
	//std::cout << "sourceID." << data->GetSourceID() << std::endl;
	SaveToRawYUVFile(data);
 

}
void ZoomSDKRenderer::onRawDataStatusChanged(RawDataStatus status)
{
	std::cout << "onRawDataStatusChanged." << std::endl;
}

void ZoomSDKRenderer::onRendererBeDestroyed()
{
	std::cout << "onRendererBeDestroyed ." << std::endl;
}

void ZoomSDKRenderer::SaveToRawYUVFile(YUVRawDataI420* data) {

	// Open the file for writing
	std::ofstream outputFile("output.yuv", std::ios::out | std::ios::binary | std::ios::app);
	if (!outputFile.is_open())
	{
		std::cout << "Error opening file." << std::endl;
		return;
	}
	// Calculate the sizes for Y, U, and V components
	size_t ySize = data->GetStreamWidth() * data->GetStreamHeight();
	size_t uvSize = ySize / 4;

	// Write Y, U, and V components to the output file
	outputFile.write(data->GetYBuffer(), ySize);
	outputFile.write(data->GetUBuffer(), uvSize);
	outputFile.write(data->GetVBuffer(), uvSize);


	// Close the file
	outputFile.close();
	outputFile.flush();
	//cout << "YUV420 buffer saved to file." << endl;
}

