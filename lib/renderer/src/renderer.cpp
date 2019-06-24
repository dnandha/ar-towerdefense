#include "renderer.h"

Renderer::Renderer(Size imsize, std::list<std::string> resources) {
    for (const auto& res : resources) {
        ovis::addResourceLocation(res);
    }

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

void Renderer::SetEntityPosition(const std::string& name, std::vector<double> pos) {
    m_winscene->setEntityPose(name, Vec3d(pos[0], pos[1], pos[2]), m_vrot);
}

void Renderer::SetEntityAnimation(const std::string& name, const std::string& animname) {
    m_winscene->playEntityAnimation(name, animname);
}

//void Renderer::SetEntityPoses() {
//}

void Renderer::UpdateView(Vec3d tvec, Vec3d rvec) {
    m_winscene->setCameraPose(tvec, rvec, false);
}

int Renderer::WaitKey(double waittime) {
    return ovis::waitKey(waittime);
}
