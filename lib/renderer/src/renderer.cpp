#include "renderer.h"

Renderer::Renderer(std::vector<int> size, std::list<std::string> resources) {
    for (const auto& res : resources) {
        ovis::addResourceLocation(res);
    }

    Size2i imsize(size[0], size[1]);

    m_winscene = ovis::createWindow(
          String("game"),
          imsize,
          ovis::SCENE_INTERACTIVE | ovis::SCENE_AA
    );

    // set camera
    double focal_length = 800.0;
    m_K = Mat1d::zeros(3, 3); // init empty matrix
    m_K.at<double>(0, 0) = focal_length; // f_x
    m_K.at<double>(1, 1) = focal_length; // f_y
    m_K.at<double>(2, 2) = 1; // f_z
    m_K = getDefaultNewCameraMatrix(m_K, imsize, true);
    m_winscene->setCameraIntrinsics(m_K, imsize);

    // set light
    m_winscene->createLightEntity("sun", Vec3i(0, 0, 100));
}

void Renderer::AddEntity(const std::string& name,
                        const std::string& meshname) {
    m_vrot = Vec3f(CV_PI, 0.0, 0.0);
    m_winscene->createEntity(name, meshname, Vec3i(0, 0, 0), m_vrot);
}

void Renderer::RemoveEntity(const std::string& name) {
    m_winscene->removeEntity(name);
}

void Renderer::SetEntityPosition(const std::string& name, std::vector<double> pos) {
    m_winscene->setEntityPose(name, Vec3d(pos[0], pos[1], pos[2]), m_vrot);
}

void Renderer::PlayEntityAnimation(const std::string& name, const std::string& animname) {
    m_winscene->playEntityAnimation(name, animname);
}

void Renderer::StopEntityAnimation(const std::string& name, const std::string& animname) {
    m_winscene->stopEntityAnimation(name, animname);
}

void Renderer::updateBackground(const Mat& image) {
	m_winscene->setBackground(image);
}

void Renderer::updateBackground(const Scalar& color) {
	m_winscene->setBackground(color);
}

void Renderer::getScreenshot(const Mat& output) {
	m_winscene->getScreenshot(output);
}

void Renderer::UpdateView(Vec3d tvec, Vec3d rvec) {
    m_winscene->setCameraPose(tvec, rvec, false);
}

//Draws Line between a Tower and an ogreentity. Needs camerapose
//void Renderer::DrawLine(const Mat& image, Tower& tow, Unit& enemy, const Vec3d& tvec, const Vec3d& rvec) {
	//Position Tower
	//Position posT = tow.GetPosition();
	//Position Enemy
	//Position posE = enemy.GetPosition();

	//Point3d posTcv;
	//Point3d posEcv;
	//posTcv.x = posT[0];
	//posTcv.y = posT[1];
	//posTcv.z = posT[2];
	//posEcv.x = posE[0];
	//posEcv.y = posE[1];
	//posEcv.z = posE[2];
	//std::vector<Point3d> pointList;
	//pointList.push_back(posTcv);
	//pointList.push_back(posEcv);
	//std::vector<Point2d> out2Dpts;
	//projectPoints(pointList, rvec, tvec, camMatrix2, distCoeffs2, out2Dpts);

	//line(image, out2Dpts[0], out2Dpts[1], Scalar(0, 0, 255));
//}

//Shows a Text. Overloaded. This one shows the text in the middle of the picture
void ShowText(const Mat image, const String text) {
	Size textSize = getTextSize(text, FONT_HERSHEY_SIMPLEX, 1, 2, 0);
	int baseLine = 2;
	
	double textX = (image.cols - textSize.width) / 2;
	double textY = (image.rows + textSize.height) / 2;

	putText(image, text, Point(textX, textY), FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 0, 0), 2);

}

//Shows a Text. Overloaded. This one shows the text at the given position(coord system starts in lower left corner).
//The point specifies the lower left corner/origin of the string
void ShowText(const Mat image, const std::string& text, const Point& pos) {
	Size textSize = getTextSize(text, FONT_HERSHEY_SIMPLEX, 1, 2, 0);
	
	putText(image, text, pos, FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 0, 0), 2);
}

int Renderer::WaitKey(double waittime) {
    return ovis::waitKey(waittime);
}
