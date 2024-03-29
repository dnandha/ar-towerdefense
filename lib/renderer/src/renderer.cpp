/*
* ARTD (Augmented Reality Tower Defense)
* Copyright (C) 2019 Jaeger,Stegmueller,Boche,Nandha 
* 
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
* 
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* 
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

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
    //double focal_length = 800.0;
    //m_K = Mat1d::zeros(3, 3); // init empty matrix
    //m_K.at<double>(0, 0) = focal_length; // f_x
    //m_K.at<double>(1, 1) = focal_length; // f_y
    //m_K.at<double>(2, 2) = 1; // f_z
    //m_K = getDefaultNewCameraMatrix(m_K, imsize, true);
    //m_winscene->setCameraIntrinsics(m_K, imsize);

    // set light
    m_winscene->createLightEntity("sun", Vec3i(0, 0, -100));
}

void Renderer::AddEntity(const std::string& name,
                        const std::string& meshname) {
    m_vrot = Vec3f(CV_PI, 0.0, 0.0);
    m_winscene->createEntity(name, meshname, Vec3i(0, 0, 0), m_vrot);
}

void Renderer::AddEntity(const std::string& name,
                        const std::string& meshname,
                        Vec3d vrot) {
    m_vrot = vrot;
    m_winscene->createEntity(name, meshname, Vec3i(0, 0, 0), m_vrot);
}

void Renderer::RemoveEntity(const std::string& name) {
    m_winscene->removeEntity(name);
}

void Renderer::SetEntityPosition(const std::string& name, Vec3d pos) {
    m_winscene->setEntityPose(name, pos, m_vrot);
}

void Renderer::SetEntityPosition(const std::string& name, Vec3d pos, Vec3d rot) {
    m_winscene->setEntityPose(name, pos, rot);
}

void Renderer::SetEntityLookAt(const std::string& name, Vec3d pos) {
    m_winscene->setEntityLookAt(name, "", pos);
}

void Renderer::SetEntityScale(const std::string& name, float scale) {
    m_winscene->setEntityProperty(name, 1, Vec3d(scale, scale, scale));
}

void Renderer::PlayEntityAnimation(const std::string& name, const std::string& animname) {
    m_winscene->playEntityAnimation(name, animname);
}

void Renderer::StopEntityAnimation(const std::string& name, const std::string& animname) {
    m_winscene->stopEntityAnimation(name, animname);
}

void Renderer::getScreenshot(const Mat& output) {
	m_winscene->getScreenshot(output);
}

void Renderer::UpdateView(Vec3d tvec, Vec3d rvec) {
    m_winscene->setCameraPose(tvec, rvec, true);
}

void Renderer::UpdateBackground(Mat bgimage, bool show) {
    m_bgimage = bgimage;
    if (show)
        m_winscene->setBackground(m_bgimage);
}

//Draws Line between a Tower and an ogreentity. Needs camerapose
void Renderer::DrawLine(const Vec3d& tower_tvec, const Vec3d& unit_tvec) {
	std::vector<Point3d> pointList;
	pointList.push_back(tower_tvec);
	pointList.push_back(unit_tvec);

	std::vector<Point2d> out2Dpts;
	projectPoints(pointList, Vec3d(0,0,0), Vec3d(0,0,0), m_K, m_Dist, out2Dpts);

	line(m_bgimage, out2Dpts[0], out2Dpts[1], Scalar(0, 0, 255), 3);
}

void Renderer::ShowText(const String& text, const Point& pos, bool centered) {
    Point pt = pos;

    Size textSize = getTextSize(text, FONT_HERSHEY_SIMPLEX, 1, 2, 0);
    if (centered) {
       double textX = (m_bgimage.cols - textSize.width) / 2;
       double textY = (m_bgimage.rows + textSize.height) / 2;
       pt = Point(textX, textY);
    }
	putText(m_bgimage, text, pt, FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 0, 0), 2);
    m_winscene->setBackground(m_bgimage);
}

int Renderer::WaitKey(double waittime) {
    return ovis::waitKey(waittime);
}
