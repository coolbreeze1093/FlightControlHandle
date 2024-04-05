#include <unordered_map>
#include <string>
class CalChannelValue
{
    struct OneCtrlValue
    {
        int16_t max=0;
        int16_t min=0;
        int16_t init=0;

        void updateValue(int16_t v)
        {
            if(min>v)
            {
                min=v;
            }
            if(max<v)
            {
                max=v;
            }
        }
    };
    
public:
    CalChannelValue();
    ~CalChannelValue();
    void initCtrlValue(int16_t value1,int16_t value2,int16_t value3,int16_t value4);
    void quitInit();
    void freshCtrlValue(int16_t value1,int16_t value2,int16_t value3,int16_t value4);
    double getChannelValue_1();
    double getChannelValue_2();
    double getChannelValue_3();
    double getChannelValue_4();

    void initChannleValue();

    std::unordered_map<std::string,OneCtrlValue>&getInitValue(){return m_initValue;}
private:
    double rangLimit(double value);

private:
    std::unordered_map<std::string,OneCtrlValue> m_initValue;
    std::unordered_map<std::string,int16_t> m_normalValue;

    const std::string m_CtrlValueName_1="ctrl1";
    const std::string m_CtrlValueName_2="ctrl2";
    const std::string m_CtrlValueName_3="ctrl3";
    const std::string m_CtrlValueName_4="ctrl4";

    bool m_initOnlyOnce=true;

    double m_channleValue_1=0;//L
    double m_channleValue_2=0;//L
    double m_channleValue_3=0;//L
    double m_channleValue_4=0;//R
};