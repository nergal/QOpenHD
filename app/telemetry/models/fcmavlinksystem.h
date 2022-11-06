#ifndef FC_MAVLINK_SYSTEM_H
#define FC_MAVLINK_SYSTEM_H

#include <QElapsedTimer>
#include <QObject>
#include <QTimer>
#include <QQmlContext>
#include <mavsdk/mavsdk.h>
#include <mavsdk/plugins/action/action.h>
#include <mavsdk/plugins/telemetry/telemetry.h>
#include "..//mavlink_include.h"

// Really nice, this way we don't have to write all the setters / getters / signals ourselves !
#include "../../../lib/lqtutils_master/lqtutils_prop.h"

/**
 * This used to be called OpenHD and was a mix of everything, it has become FCMavlinkSystem -
 * A QT model for (fire and forget) data from the mavlink FC connected to the air unit.
 * OpenHD always has only one connected FC, so this can be a singleton, too.
 * Also, note that nothing OpenHD specific should ever make it into here - OpenHD supports raw access to
 * the Flight Controller, but it is NOT a Flight Controller ;)
 * The corresponding qml element is called _fcMavlinkSystem.
 *
 * NOTE: R.n we are using both the mavsdk "subscription" pattern for a couble of values and the humungus
 * mavlink message type switch-case (legacy). This is a bit confusing, not sure if we should either not
 * use mavsdk subscriptions at all or use them as much as possible.
 */
class FCMavlinkSystem : public QObject
{
    Q_OBJECT
    // These members can be written & read from c++, but are only readable from qml (which is a common recommendation for QT application(s)).
    // Aka we just set them in c++ by calling the setter declared from the macro, which then emits the changed signal if needed
    // and therefore updates the corresponding UI element in QT
    // Note that this macro is short for the following common pattern:
    //
    // private:
    //    int m_some_value=0;
    // signals:
    //    some_value_changed(int some_value)
    // public:
    // void set_some_value(int value){
    //    if(m_some_value==value)return;
    //    m_some_value=value;
    //    emit some-value_changed();
    // }
    //
    L_RO_PROP(double, battery_current, set_battery_current, 0)
    L_RO_PROP(double, battery_voltage, set_battery_voltage, 0)
    L_RO_PROP(int, battery_percent, set_battery_percent, 0)
    // same as battery_percent, but as an "icon"
    L_RO_PROP(QString, battery_percent_gauge, set_battery_percent_gauge, "\uf091")
    // roll, pitch and yaw
    L_RO_PROP(double, pitch, set_pitch, 0)
    L_RO_PROP(double, roll, set_roll, 0)
    L_RO_PROP(double, yaw, set_yaw, 0)
    // mixed
    L_RO_PROP(double, throttle, set_throttle, 0)
    L_RO_PROP(float,vibration_x,set_vibration_x,0)
    L_RO_PROP(float,vibration_y,set_vibration_y,0)
    L_RO_PROP(float,vibration_z,set_vibration_z,0)
    // see alive timer
    L_RO_PROP(bool,is_alive,set_is_alive,false)
    //
    L_RO_PROP(double,lat,set_lat,0.0)
    L_RO_PROP(double,lon,set_lon,0.0)
    L_RO_PROP(int,satellites_visible,set_satellites_visible,0)
    L_RO_PROP(double,gps_hdop,set_gps_hdop,99.0)
    L_RO_PROP(int,gps_fix_type,set_gps_fix_type,0)
    L_RO_PROP(double,vx,set_vx,0.0)
    L_RO_PROP(double,vy,set_vy,0.0)
    L_RO_PROP(double,vz,set_vz,0.0)
    //
    L_RO_PROP(double,alt_rel,set_alt_rel,0.0)
    L_RO_PROP(double,alt_msl,set_alt_msl,0.0)
    L_RO_PROP(int,flight_mah,set_flight_mah,0)
    L_RO_PROP(int,app_mah,set_app_mah,0)
    L_RO_PROP(int,mah_km,set_mah_km,0)
    //
    L_RO_PROP(double,vehicle_vx_angle,set_vehicle_vx_angle,0.0);
    L_RO_PROP(double,vehicle_vy_angle,set_vehicle_vy_angle,0.0);
    L_RO_PROP(double,vehicle_vz_angle,set_vehicle_vz_angle,0.0);
    ////
    L_RO_PROP(double,wind_speed,set_wind_speed,0)
    L_RO_PROP(double,wind_direction,set_wind_direction,0)
    L_RO_PROP(float,mav_wind_direction,set_mav_wind_direction,0)
    L_RO_PROP(float,mav_wind_speed,set_mav_wind_speed,0)
    L_RO_PROP(int,rc_rssi,set_rc_rssi,0);
    L_RO_PROP(int,imu_temp,set_imu_temp,0);
    L_RO_PROP(int,press_temp,set_press_temp,0);
    L_RO_PROP(int,esc_temp,set_esc_temp,0);
    L_RO_PROP(QString,flight_time,set_flight_time,"00:00")
    L_RO_PROP(double,flight_distance,set_flight_distance,0)
    L_RO_PROP(double,lateral_speed,set_lateral_speed,0)  
    L_RO_PROP(double,home_distance,set_home_distance,0)
    L_RO_PROP(int,boot_time,set_boot_time,0)
    L_RO_PROP(int,hdg,set_hdg,0)
    L_RO_PROP(double,speed,set_speed,0)
    //
    L_RO_PROP(double,airspeed,set_airspeed,0)
    L_RO_PROP(float,clipping_x,set_clipping_x,0.0)
    L_RO_PROP(float,clipping_y,set_clipping_y,0.0)
    L_RO_PROP(float,clipping_z,set_clipping_z,0.0)
    L_RO_PROP(float,vsi,set_vsi,0.0)
public:
    explicit FCMavlinkSystem(QObject *parent = nullptr);
    // singleton for accessing the model from c++
    static FCMavlinkSystem& instance();
    // Called in main.cpp to egister the models for qml
    static void register_for_qml(QQmlContext* qml_context);
public:
    // Process a new telemetry message coming from the FC mavlink system
    // return true if we know what to do with this message type (aka this message type has been consumed)
    bool process_message(const mavlink_message_t& msg);
    // mavlink sys id of the FC. Pretty much always 1, but it is not a hard requirement that FC always use a sys id of 1.
    // If the FC has not been discovered yet (mavsdk::system not yet set), return std::nullopt.
    std::optional<uint8_t> get_fc_sys_id();
    // Set the mavlink system reference, once discovered.
    // If we can get a telemetry value (e.g. the altitude) by subscribing to a mavlink message this is preferred over
    // manually parsing the message, and we register the callbacks to mavsdk when this is called (since we need the "system"
    // reference for it)
    void set_system(std::shared_ptr<mavsdk::System> system);
public:
    void telemetryStatusMessage(QString message, int level);
    void calculate_home_distance();
    void calculate_home_course();

    /* public so that a QTimer can call it from main(), temporary fix due to some quirks with
       the way QTimer and QML singletons/context properties work */
    void updateFlightTimer();

    void findGcsPosition();
    void updateFlightDistance();
    void updateAppMah();
    void updateAppMahKm();
    void updateVehicleAngles();
    void updateWind();

    Q_PROPERTY(int home_course MEMBER m_home_course WRITE set_home_course NOTIFY home_course_changed)
    void set_home_course(int home_course);

    Q_PROPERTY(int home_heading MEMBER m_home_heading WRITE set_home_heading NOTIFY home_heading_changed)
    void set_home_heading(int home_heading);

    Q_PROPERTY(bool armed MEMBER m_armed WRITE set_armed NOTIFY armed_changed)
    void set_armed(bool armed);

    Q_PROPERTY(QString flight_mode MEMBER m_flight_mode WRITE set_flight_mode NOTIFY flight_mode_changed)
    void set_flight_mode(QString flight_mode);

    Q_PROPERTY(QString mav_type MEMBER m_mav_type WRITE set_mav_type NOTIFY mav_type_changed)
    void set_mav_type(QString mav_type);

    Q_PROPERTY(double homelat MEMBER m_homelat WRITE set_homelat NOTIFY homelat_changed)
    void set_homelat(double homelat);

    Q_PROPERTY(double homelon MEMBER m_homelon WRITE set_homelon NOTIFY homelon_changed)
    void set_homelon(double homelon);

    Q_PROPERTY(int current_waypoint MEMBER m_current_waypoint WRITE setCurrentWaypoint NOTIFY currentWaypointChanged)
    void setCurrentWaypoint(int current_waypoint);

    Q_PROPERTY(int total_waypoints MEMBER m_total_waypoints WRITE setTotalWaypoints NOTIFY totalWaypointsChanged)
    void setTotalWaypoints(int total_waypoints);

    Q_PROPERTY(QString last_ping_result_flight_ctrl MEMBER  m_last_ping_result_flight_ctrl WRITE set_last_ping_result_flight_ctrl NOTIFY last_ping_result_flight_ctrl_changed)
    void set_last_ping_result_flight_ctrl(QString last_ping_result_flight_ctrl);

    // Set to true if this FC supports basic commands, like return to home usw
    // R.N we only show those commands in the UI if this flag is set
    // and the flag is set if the FC is PX4 or Ardupilot
    // NOTE: this used to be done by .mav_type == "ARDUPLANE" ... in qml - please avoid that, just add another qt boolean here
    // (for example is_copter, is_plane or similar)
    Q_PROPERTY(bool supports_basic_commands MEMBER  m_supports_basic_commands WRITE set_supports_basic_commands NOTIFY supports_basic_commands_changed)
    void set_supports_basic_commands(bool supports_basic_commands);
signals:
    // mavlink
    void armed_changed(bool armed);
    void flight_mode_changed(QString flight_mode);
    void mav_type_changed(QString mav_type);
    void homelat_changed(double homelat);
    void homelon_changed(double homelon);
    void home_course_changed(int home_course);
    void home_heading_changed(int home_heading);
    void messageReceived(QString message, int level);

    //void throttle_changed(double throttle);

    void vibration_x_changed(float vibration_x);
    void vibration_y_changed(float vibration_y);
    void vibration_z_changed(float vibration_z);

    void rcChannelChanged(int channelIdx,int value);

    void currentWaypointChanged (int current_waypoint);
    void totalWaypointsChanged (int total_waypoints);

    void last_ping_result_flight_ctrl_changed(QString last_ping_result_flight_ctrl);
    void supports_basic_commands_changed(bool supports_basic_commands);
public:
    // mavlink
    bool m_armed = false;
    QString m_flight_mode = "------";
    QString m_mav_type = "UNKOWN";

    double m_homelat = 0.0;
    double m_homelon = 0.0;
    bool gcs_position_set = false;
    int gps_quality_count = 0;

    int m_home_heading = 0; //this is actual global heading
    int m_home_course = 0; //this is the relative course from nose

    double speed_last_time = 0.0;

    qint64 flightDistanceLastTime= 0;
    long total_dist= 0;

    qint64 mahLastTime= 0;
    qint64 mahKmLastTime= 0;
    double total_mah= 0;

    QElapsedTimer totalTime;
    QElapsedTimer flightTimeStart;

    QTimer* timer = nullptr;

    int m_current_waypoint = 0;
    int m_total_waypoints = 0;

    int m_mode = 0;

    int m_arm_disarm = 99;

    int m_reboot_shutdown=99;
    QString m_last_ping_result_flight_ctrl="NA";
    bool m_supports_basic_commands=true;
private:
    // NOTE: Null until system discovered
    std::shared_ptr<mavsdk::System> _system=nullptr;
    std::shared_ptr<mavsdk::Action> _action=nullptr;
    std::shared_ptr<mavsdk::Telemetry> _mavsdk_telemetry=nullptr;
public:
    //
    // Try to change the arming state. Once completed, since we listen to arm/disarm results,
    // the armed status is changed. On failure, a message is pushed onto the HUD
    Q_INVOKABLE void arm_fc_async(bool disarm=false);
    Q_INVOKABLE void send_return_to_launch_async();
    Q_INVOKABLE bool send_command_reboot(bool reboot);
    // -----------------------
public:
    Q_PROPERTY(qint64 last_heartbeat MEMBER m_last_heartbeat WRITE set_last_heartbeat NOTIFY last_heartbeat_changed)
    void set_last_heartbeat(qint64 last_heartbeat);
public:
    qint64 m_last_heartbeat = -1;
    QTimer* m_alive_timer = nullptr;
signals:
    void last_heartbeat_changed(qint64 last_heartbeat);
    void is_alive_changed(bool alive);
private:
    void update_alive();
};



#endif // FC_MAVLINK_SYSTEM_H
