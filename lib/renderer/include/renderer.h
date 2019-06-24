#include <opencv2/calib3d.hpp>
#include <opencv2/ovis.hpp>

#include <list>
#include <vector>

using namespace cv;

class Renderer {
  Mat m_K;
  Ptr<ovis::WindowScene> m_winscene;
  Vec3d m_vrot;

  public:
    Renderer(Size2i, std::list<std::string> resources);

    void AddEntity(const std::string& name,
                const std::string& meshname);

    void SetEntityPosition(const std::string& name,
                       std::vector<double> pos);

    void SetEntityAnimation(const std::string& name,
                       const std::string& animname);

    void UpdateView(Vec3d tvec, Vec3d rvec);
    int WaitKey(double time);
};

