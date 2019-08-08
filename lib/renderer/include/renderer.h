#include <opencv2/calib3d.hpp>
#include <opencv2/ovis.hpp>
//Für DrawLine. Funzt so aber nicht
//#include "unit.h"
//#include "tower.h"

#include <list>
#include <vector>

using namespace cv;

class Renderer {
  Mat m_K;
  Ptr<ovis::WindowScene> m_winscene;
  Vec3d m_vrot;

  public:
    Renderer(std::vector<int> size, std::list<std::string> resources);

    void AddEntity(const std::string& name,
                const std::string& meshname);

    void RemoveEntity(const std::string& name);

    void SetEntityPosition(const std::string& name,
                       std::vector<double> pos);

    void PlayEntityAnimation(const std::string& name,
                       const std::string& animname);

    void StopEntityAnimation(const std::string& name,
                       const std::string& animname);

					   
	void updateBackground(const Mat& image);

	void updateBackground(const Scalar& color);

	void getScreenshot(const Mat& out);
	
    void UpdateView(Vec3d tvec, Vec3d rvec);
	
	//void DrawLine(const Mat& image, Tower& tow, Unit& target, const Vec3d& tvec, const Vec3d& rvec);

	void ShowText(const Mat& image, const String& text);

	void ShowText(const Mat& image, const String& text, const Point& pos);
	
    int WaitKey(double time);
};

