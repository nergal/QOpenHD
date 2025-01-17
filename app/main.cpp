#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlComponent>
#include <QDebug>
#include <QFontDatabase>
#if defined(__android__)
#include <QtAndroidExtras/QtAndroid>
const QVector<QString> permissions({"android.permission.INTERNET",
                                    "android.permission.WRITE_EXTERNAL_STORAGE",
                                    "android.permission.READ_EXTERNAL_STORAGE",
                                    "android.permission.ACCESS_NETWORK_STATE",
                                    "android.permission.ACCESS_FINE_LOCATION"});
#endif

#include "telemetry/models/fcmavlinksystem.h"
#include "telemetry/models/aohdsystem.h"
#include "telemetry/MavlinkTelemetry.h"
#include "telemetry/models/rcchannelsmodel.h"

#include "util/QmlObjectListModel.h"

#include "osd/speedladder.h"
#include "osd/altitudeladder.h"
#include "osd/headingladder.h"
#include "osd/horizonladder.h"
#include "osd/flightpathvector.h"
#include "osd/drawingcanvas.h"
#include "osd/aoagauge.h"
//
#ifdef QOPENHD_ENABLE_VIDEO_VIA_AVCODEC
#include "exp/QSGVideoTextureItem.h"
#endif
#include "util/qrenderstats.h"


#if defined(__ios__)
#include "platform/appleplatform.h"
#endif

#if defined(ENABLE_GSTREAMER)
#include "videostreaming/gst_qmlglsink/gstvideostream.h"
#include "videostreaming/gst_qmlglsink/gst_helper.hpp"
#endif

#include "videostreaming/QOpenHDVideoHelper.hpp"
#include "videostreaming/decodingstatistcs.h"


#include "logging/logmessagesmodel.h"
#include "logging/hudlogmessagesmodel.h"
#include "telemetry/settings/mavlinksettingsmodel.h"
#include "telemetry/settings/synchronizedsettings.h"
#include "qopenhd.h"
#include "util/WorkaroundMessageBox.h"

#ifdef QOPENHD_ENABLE_ADSB_LIBRARY
#include "adsb/ADSBVehicleManager.h"
#include "adsb/ADSBVehicle.h"
#endif


// Load all the fonts we use ?!
static void load_fonts(){
    QFontDatabase::addApplicationFont(":/resources/Font Awesome 5 Free-Solid-900.otf");
    QFontDatabase::addApplicationFont(":/resources/osdicons.ttf");
    QFontDatabase::addApplicationFont(":/resources/materialdesignicons-webfont.ttf");
    //
    QFontDatabase::addApplicationFont(":/osdfonts/Acme-Regular.ttf");
    QFontDatabase::addApplicationFont(":/osdfonts/Aldrich-Regular.ttf");
    QFontDatabase::addApplicationFont(":/osdfonts/AnonymousPro-Bold.ttf");
    QFontDatabase::addApplicationFont(":/osdfonts/AnonymousPro-BoldItalic.ttf");
    QFontDatabase::addApplicationFont(":/osdfonts/Archivo-Bold.ttf");
    QFontDatabase::addApplicationFont(":/osdfonts/Archivo-Medium.ttf");
    QFontDatabase::addApplicationFont(":/osdfonts/Archivo-Regular.ttf");
    QFontDatabase::addApplicationFont(":/osdfonts/ArchivoBlack-Regular.ttf");
    QFontDatabase::addApplicationFont(":/osdfonts/Armata-Regular.ttf");
    QFontDatabase::addApplicationFont(":/osdfonts/Bangers-Regular.ttf");
    QFontDatabase::addApplicationFont(":/osdfonts/BlackOpsOne-Regular.ttf");
    QFontDatabase::addApplicationFont(":/osdfonts/Bungee-Regular.ttf");
    QFontDatabase::addApplicationFont(":/osdfonts/Carbon-Bold.ttf");
    QFontDatabase::addApplicationFont(":/osdfonts/Chicle-Regular.ttf");
    QFontDatabase::addApplicationFont(":/osdfonts/Digital7SegmentDisplay.ttf");
    QFontDatabase::addApplicationFont(":/osdfonts/DigitalDotDisplay.ttf");
    QFontDatabase::addApplicationFont(":/osdfonts/DigitalSubwayTicker.ttf");
    QFontDatabase::addApplicationFont(":/osdfonts/ExpletusSans-Bold.ttf");
    QFontDatabase::addApplicationFont(":/osdfonts/FjallaOne-Regular.ttf");
    QFontDatabase::addApplicationFont(":/osdfonts/FredokaOne-Regular.ttf");
    QFontDatabase::addApplicationFont(":/osdfonts/GeostarFill-Regular.ttf");
    QFontDatabase::addApplicationFont(":/osdfonts/Iceberg-Regular.ttf");
    QFontDatabase::addApplicationFont(":/osdfonts/Iceland-Regular.ttf");
    QFontDatabase::addApplicationFont(":/osdfonts/Jura-Bold.ttf");
    QFontDatabase::addApplicationFont(":/osdfonts/KeaniaOne-Regular.ttf");
    QFontDatabase::addApplicationFont(":/osdfonts/Larabie.ttf");
    QFontDatabase::addApplicationFont(":/osdfonts/LuckiestGuy-Regular.ttf");
    QFontDatabase::addApplicationFont(":/osdfonts/Merysha-Regular.ttf");
    QFontDatabase::addApplicationFont(":/osdfonts/NixieOne-Regular.ttf");
    QFontDatabase::addApplicationFont(":/osdfonts/Orbitron-Bold.ttf");
    QFontDatabase::addApplicationFont(":/osdfonts/Orbitron-Regular.ttf");
    QFontDatabase::addApplicationFont(":/osdfonts/Oxygen-Bold.ttf");
    QFontDatabase::addApplicationFont(":/osdfonts/Oxygen-Regular.ttf");
    QFontDatabase::addApplicationFont(":/osdfonts/PassionOne-Bold.ttf");
    QFontDatabase::addApplicationFont(":/osdfonts/Quantico-Bold.ttf");
    QFontDatabase::addApplicationFont(":/osdfonts/Quantico-Regular.ttf");
    QFontDatabase::addApplicationFont(":/osdfonts/Quicksand-Bold.ttf");
    QFontDatabase::addApplicationFont(":/osdfonts/Quicksand-Regular.ttf");
    QFontDatabase::addApplicationFont(":/osdfonts/RammettoOne-Regular.ttf");
    QFontDatabase::addApplicationFont(":/osdfonts/Rationale-Regular.ttf");
    QFontDatabase::addApplicationFont(":/osdfonts/Righteous-Regular.ttf");
    QFontDatabase::addApplicationFont(":/osdfonts/RobotoMono-Bold.ttf");
    QFontDatabase::addApplicationFont(":/osdfonts/RobotoMono-Regular.ttf");
    QFontDatabase::addApplicationFont(":/osdfonts/RussoOne-Regular.ttf");
    QFontDatabase::addApplicationFont(":/osdfonts/ShareTech-Regular.ttf");
    QFontDatabase::addApplicationFont(":/osdfonts/ShareTechMono-Regular.ttf");
    QFontDatabase::addApplicationFont(":/osdfonts/SigmarOne-Regular.ttf");
    QFontDatabase::addApplicationFont(":/osdfonts/Slackey-Regular.ttf");
    QFontDatabase::addApplicationFont(":/osdfonts/UbuntuMono-Bold.ttf");
    QFontDatabase::addApplicationFont(":/osdfonts/UbuntuMono-BoldItalic.ttf");
    QFontDatabase::addApplicationFont(":/osdfonts/Visitor.ttf");
    QFontDatabase::addApplicationFont(":/osdfonts/ZolanMonoOblique.ttf");
}

// Write context properties for all platforms
static void write_platform_context_properties(QQmlApplicationEngine& engine){
#if defined(__android__)
    engine.rootContext()->setContextProperty("IsAndroid", QVariant(true));
#else
    engine.rootContext()->setContextProperty("IsAndroid", QVariant(false));
#endif

#if defined(__ios__)
    engine.rootContext()->setContextProperty("IsiOS", QVariant(true));
#else
    engine.rootContext()->setContextProperty("IsiOS", QVariant(false));
#endif

#if defined(__windows__)
    engine.rootContext()->setContextProperty("IsWindows", QVariant(true));
#else
    engine.rootContext()->setContextProperty("IsWindows", QVariant(false));
#endif

#if defined(__desktoplinux__)
    engine.rootContext()->setContextProperty("IsDesktopLinux", QVariant(true));
#else
    engine.rootContext()->setContextProperty("IsDesktopLinux", QVariant(false));
#endif

#if defined(__macos__)
    engine.rootContext()->setContextProperty("IsMac", QVariant(true));
#else
    engine.rootContext()->setContextProperty("IsMac", QVariant(false));
#endif

#if defined(__rasp_pi__)
    engine.rootContext()->setContextProperty("IsRaspPi", QVariant(true));
#else
    engine.rootContext()->setContextProperty("IsRaspPi", QVariant(false));
#endif
}

void write_other_context_properties(QQmlApplicationEngine& engine){
#ifdef ENABLE_GSTREAMER
    engine.rootContext()->setContextProperty("EnableGStreamer", QVariant(true));
#else
    engine.rootContext()->setContextProperty("EnableGStreamer", QVariant(false));
#endif
#ifdef ENABLE_VIDEO_RENDERER
    engine.rootContext()->setContextProperty("EnableVideoRender", QVariant(true));
#else
     engine.rootContext()->setContextProperty("EnableVideoRender", QVariant(false));
#endif
}

int main(int argc, char *argv[]) {
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    //QCoreApplication::setAttribute(Qt::AA_UseOpenGLES);

    QCoreApplication::setOrganizationName("Open.HD");
    QCoreApplication::setOrganizationDomain("open.hd");
    QCoreApplication::setApplicationName("QOpenHD");

#ifdef ENABLE_GSTREAMER
    initGstreamerOrThrowExtra(argc,argv);
    initQmlGlSinkOrThrow();
#endif //ENABLE_GSTREAMER

    QSettings settings;

    const double global_scale = settings.value("global_scale", 1.0).toDouble();
    const std::string global_scale_s = std::to_string(global_scale);
    QByteArray scaleAsQByteArray(global_scale_s.c_str(), global_scale_s.length());
    qputenv("QT_SCALE_FACTOR", scaleAsQByteArray);

    // https://doc.qt.io/qt-6/qtquick-visualcanvas-scenegraph-renderer.html
    //qputenv("QSG_VISUALIZE", "overdraw");
   // QCoreApplication::setAttribute(Qt::AA_UseOpenGLES);
    //QLoggingCategory::setFilterRules("qt.scenegraph.*=true");
    //QLoggingCategory::setFilterRules("qt.scenegraph.time.*=true");
    //QLoggingCategory::setFilterRules("qt.scenegraph.general=true");
    //QLoggingCategory::setFilterRules("qt.scenegraph.time.texture=true");
    //QLoggingCategory::setFilterRules("qt.scenegraph.time.renderloop=true");
    //QLoggingCategory::setFilterRules("qt.qpa.eglfs.*=true");
    //QLoggingCategory::setFilterRules("qt.qpa.egl*=true");


    QApplication app(argc, argv);

#if defined(__ios__)
    auto applePlatform = ApplePlatform::instance();
    applePlatform->disableScreenLock();
    applePlatform->registerNotifications();
#endif


#if defined(__android__)
    OpenHDUtil::instance().keep_screen_on(true);

    for(const QString &permission : permissions) {
        auto result = QtAndroid::checkPermission(permission);

        if (result == QtAndroid::PermissionResult::Denied) {
            auto resultHash = QtAndroid::requestPermissionsSync(QStringList({permission}));
            if (resultHash[permission] == QtAndroid::PermissionResult::Denied) {
                return 0;
            }
        }
    }
#endif

#if defined(__rasp_pi__)
    qDebug() << "Initializing Pi";
    OpenHDPi pi;

    // set persistent brightness level at startup
    if (pi.is_raspberry_pi()) {
        auto brightness = settings.value("brightness", 100).toInt();
        pi.set_brightness(brightness);
    }
    qDebug() << "Finished initializing Pi";
#endif

    load_fonts();

    qmlRegisterUncreatableType<QmlObjectListModel>("OpenHD", 1, 0, "QmlObjectListModel", "Reference only");

    qmlRegisterType<SpeedLadder>("OpenHD", 1, 0, "SpeedLadder");
    qmlRegisterType<AltitudeLadder>("OpenHD", 1, 0, "AltitudeLadder");
    qmlRegisterType<HeadingLadder>("OpenHD", 1, 0, "HeadingLadder");
    qmlRegisterType<HorizonLadder>("OpenHD", 1, 0, "HorizonLadder");
    qmlRegisterType<FlightPathVector>("OpenHD", 1, 0, "FlightPathVector");
    qmlRegisterType<DrawingCanvas>("OpenHD", 1, 0, "DrawingCanvas");
    qmlRegisterType<AoaGauge>("OpenHD", 1, 0, "AoaGauge");

    QQmlApplicationEngine engine;
#ifdef QOPENHD_ENABLE_VIDEO_VIA_AVCODEC
    // QT doesn't have the define(s) from c++
    engine.rootContext()->setContextProperty("QOPENHD_ENABLE_VIDEO_VIA_AVCODEC", QVariant(true));
    qmlRegisterType<QSGVideoTextureItem>("OpenHD", 1, 0, "QSGVideoTextureItem");
#else
    engine.rootContext()->setContextProperty("QOPENHD_ENABLE_VIDEO_VIA_AVCODEC", QVariant(false));
#endif
    engine.rootContext()->setContextProperty("_qopenhd", &QOpenHD::instance());
    QOpenHD::instance().setEngine(&engine);

    engine.rootContext()->setContextProperty("_qrenderstats", &QRenderStats::instance());

    write_platform_context_properties(engine);
    write_other_context_properties(engine);
    engine.rootContext()->setContextProperty("_logMessagesModel", &LogMessagesModel::instance());
    engine.rootContext()->setContextProperty("_hudLogMessagesModel", &HUDLogMessagesModel::instance());

    engine.rootContext()->setContextProperty("_airCameraSettingsModel", &MavlinkSettingsModel::instanceAirCamera());
    engine.rootContext()->setContextProperty("_airPiSettingsModel", &MavlinkSettingsModel::instanceAir());
    engine.rootContext()->setContextProperty("_groundPiSettingsModel", &MavlinkSettingsModel::instanceGround());
    engine.rootContext()->setContextProperty("_synchronizedSettings", &SynchronizedSettings::instance());

#if defined(ENABLE_GSTREAMER)
#if defined(ENABLE_MAIN_VIDEO)
    //std::unique_ptr<GstVideoStream>  mainVideo=nullptr;
    std::unique_ptr<GstVideoStream>  mainVideo=std::make_unique<GstVideoStream>();
    engine.rootContext()->setContextProperty("_mainVideo", mainVideo.get());
#endif
#endif

    //MavlinkTelemetry::register_for_qml(engine.rootContext());
    engine.rootContext()->setContextProperty("_mavlinkTelemetry", &MavlinkTelemetry::instance());

    // Regster all the QT Mavlink system model(s)
    // it is a common practice for QT to prefix models from c++ with an underscore
    //FCMavlinkSystem::register_for_qml(engine.rootContext());
    engine.rootContext()->setContextProperty("_fcMavlinkSystem", &FCMavlinkSystem::instance());

    engine.rootContext()->setContextProperty("_rcchannelsmodelground", &RCChannelsModel::instanceGround());

    //AOHDSystem::register_for_qml(engine.rootContext());
    engine.rootContext()->setContextProperty("_ohdSystemAir", &AOHDSystem::instanceAir());
    engine.rootContext()->setContextProperty("_ohdSystemGround", &AOHDSystem::instanceGround());

    engine.rootContext()->setContextProperty("_decodingStatistics",&DecodingStatistcs::instance());
    // dirty
    engine.rootContext()->setContextProperty("_messageBoxInstance", &workaround::MessageBox::instance());

#if defined(ENABLE_MAIN_VIDEO)
    engine.rootContext()->setContextProperty("EnableMainVideo", QVariant(true));
#else
    engine.rootContext()->setContextProperty("EnableMainVideo", QVariant(false));
#endif

//#if defined(LIMIT_ADSB_MAX)
engine.rootContext()->setContextProperty("LimitADSBMax", QVariant(true));
//#else
//engine.rootContext()->setContextProperty("LimitADSBMax", QVariant(false));
//#endif

#ifdef QOPENHD_ENABLE_ADSB_LIBRARY
engine.rootContext()->setContextProperty("QOPENHD_ENABLE_ADSB_LIBRARY", QVariant(true));
engine.rootContext()->setContextProperty("EnableADSB", QVariant(true));
auto adsbVehicleManager = ADSBVehicleManager::instance();
engine.rootContext()->setContextProperty("AdsbVehicleManager", adsbVehicleManager);
//QObject::connect(openHDSettings, &OpenHDSettings::groundStationIPUpdated, adsbVehicleManager, &ADSBVehicleManager::setGroundIP, Qt::QueuedConnection);
adsbVehicleManager->onStarted();
#else
engine.rootContext()->setContextProperty("QOPENHD_ENABLE_ADSB_LIBRARY", QVariant(false));
engine.rootContext()->setContextProperty("EnableADSB", QVariant(false));
#endif


#if defined(ENABLE_CHARTS)
    engine.rootContext()->setContextProperty("EnableCharts", QVariant(true));
#else
    engine.rootContext()->setContextProperty("EnableCharts", QVariant(false));
#endif

#if defined(ENABLE_RC)
    engine.rootContext()->setContextProperty("EnableRC", QVariant(true));
    //QJoysticks* jinstance = QJoysticks::getInstance();
    auto QJoysticks = QJoysticks::getInstance();
    engine.rootContext()->setContextProperty("QJoysticks", QJoysticks);
#else
    engine.rootContext()->setContextProperty("EnableRC", QVariant(false));
#endif

#if defined(__ios__) || defined(__android__)
    engine.rootContext()->setContextProperty("UseFullscreen", QVariant(true));
#else
    engine.rootContext()->setContextProperty("UseFullscreen", QVariant(false));
#endif

    engine.rootContext()->setContextProperty("QOPENHD_VERSION", QVariant(QOPENHD_VERSION));

    engine.load(QUrl(QLatin1String("qrc:/main.qml")));

#if defined(__android__)
    QtAndroid::hideSplashScreen();
#endif

    qDebug() << "Running QML";

#if defined(ENABLE_GSTREAMER)
#if defined(ENABLE_MAIN_VIDEO)
    const auto windowPrimary=find_qt_video_window(engine,true);
    if(windowPrimary==nullptr){
        qDebug()<<"Error primary window enabled but not found";
        //throw std::runtime_error("Window not found");
    }else{
        if(mainVideo){
            mainVideo->init(windowPrimary,true);
        }
    }
#endif
#endif

    LogMessagesModel::instance().addLogMessage("QOpenHD","running");
    const int retval = app.exec();

#if defined(ENABLE_GSTREAMER)
#if defined(ENABLE_MAIN_VIDEO)
    if(mainVideo!=nullptr){
         mainVideo->stopVideoSafe();
    }
#endif
#endif
    return retval;


}
