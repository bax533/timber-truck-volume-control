
namespace TimberControl
{
    class VolumeCalculator
    {
        const char *imgPath;
    public:
        VolumeCalculator(const char* imgPath)
        :imgPath(imgPath)
        {}
      
        double Calculate();
  };
}
