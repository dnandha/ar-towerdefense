#include <opencv2/calib3d.hpp>
#include <opencv2/ovis.hpp>
#include <opencv2/imgproc.hpp>
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
  Mat m_bgimage;

  public:
    Renderer(std::vector<int> size, std::list<std::string> resources);

    void AddEntity(const std::string& name,
                const std::string& meshname);
    void AddEntity(const std::string& name,
                const std::string& meshname,
                Vec3d orientation);

    void RemoveEntity(const std::string& name);

    void SetEntityPosition(const std::string& name, Vec3d pos);
    void SetEntityPosition(const std::string& name, Vec3d pos, Vec3d rot);

    void PlayEntityAnimation(const std::string& name,
                       const std::string& animname);

    void StopEntityAnimation(const std::string& name,
                       const std::string& animname);

	void getScreenshot(const Mat& out);
	
    void UpdateView(Vec3d tvec, Vec3d rvec);

    void UpdateBackground(Mat bgimg);

	void ShowText(const Mat& image, const String& text);

    /**
     * Shows a Text. Overloaded. This one shows the text at the given position(coord system starts in upper left corner).
     * The point specifies the lower left corner/origin of the string
     **/
	void ShowText(const String& text, const Point& pos);

    int WaitKey(double time);
};

