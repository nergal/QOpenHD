#include "mavlinksettingsmodel.h"
#include "qdebug.h"
#include "../openhd_defines.hpp"

#include <QMessageBox>
#include <QVariant>

MavlinkSettingsModel &MavlinkSettingsModel::instanceAirCamera()
{
    static MavlinkSettingsModel* instanceAirCamera=new MavlinkSettingsModel(OHD_SYS_ID_AIR,OHD_COMP_ID_AIR_CAMERA);
    return *instanceAirCamera;
}

MavlinkSettingsModel &MavlinkSettingsModel::instanceAir()
{
    static MavlinkSettingsModel* instanceAir=new MavlinkSettingsModel(OHD_SYS_ID_AIR,OHD_COMP_ID_LINK_PARAM);
    return *instanceAir;
}
MavlinkSettingsModel &MavlinkSettingsModel::instanceGround()
{
    static MavlinkSettingsModel* instanceGround=new MavlinkSettingsModel(OHD_SYS_ID_GROUND,OHD_COMP_ID_LINK_PARAM);
    return *instanceGround;
}

std::map<std::string, void *> MavlinkSettingsModel::get_whitelisted_params()
{
    std::map<std::string,void*> ret{};
    ret["WB_FREQUENCY"]=nullptr;
    ret["WB_CHANNEL_W"]=nullptr;
    ret["WB_MCS_INDEX"]=nullptr;
    //ret[""]=nullptr;
    return ret;
}

bool MavlinkSettingsModel::is_param_whitelisted(const std::string param_id)
{
    if(param_id.empty()){
        return false;
    }
    const auto tmp=get_whitelisted_params();
    if(tmp.find(param_id)!=tmp.end()){
        return true;
    }
    return false;
}

MavlinkSettingsModel::MavlinkSettingsModel(uint8_t sys_id,uint8_t comp_id,QObject *parent)
    : QAbstractListModel(parent),_sys_id(sys_id),_comp_id(comp_id)
{
#ifndef X_USE_MAVSDK
    m_data.push_back({"VIDEO_WIDTH",0});
    m_data.push_back({"VIDEO_HEIGHT",1});
    m_data.push_back({"VIDEO_FPS",1});
#endif
}

static void makePopupMessage(QString message){
    QMessageBox msgBox;
    msgBox.setText(message);
    msgBox.exec();
}

#ifdef X_USE_MAVSDK
void MavlinkSettingsModel::set_param_client(std::shared_ptr<mavsdk::System> system)
{
    // only allow adding the param client once it is discovered, do not overwrite it once discovered.
    assert(this->param_client==nullptr);
    assert(system->get_system_id()==_sys_id);
    this->param_client=std::make_shared<mavsdk::Param>(system,_comp_id,true);
    try_fetch_all_parameters();
}
#endif

bool MavlinkSettingsModel::try_fetch_all_parameters()
{
    qDebug()<<"MavlinkSettingsModel::try_fetch_all_parameters()";
    if(param_client==nullptr){
        // not discovered yet
        makePopupMessage("OHD System not found");
    }
    if(param_client){
        // first, remove anything the QT model has cached
        while(rowCount()>0){
            removeData(rowCount()-1);
        }
        qDebug()<<"Done removing old params";
        // now fetch all params using mavsdk (this talks to the OHD system(s).
        const auto params=param_client->get_all_params(true);
        for(const auto& int_param:params.int_params){
            MavlinkSettingsModel::SettingData data{QString(int_param.name.c_str()),int_param.value};
            addData(data);
        }
        if(!params.int_params.empty()){
            return true;
        }
    }else{
        // not dscovered yet
    }
    return false;
}

bool MavlinkSettingsModel::try_fetch_parameter(QString param_id)
{
    qDebug()<<"try_fetch_parameter:"<<param_id;
    if(param_client){
        const auto result=param_client->get_param_int(param_id.toStdString());
        if(result.first==mavsdk::Param::Result::Success){
            auto new_value=result.second;
            MavlinkSettingsModel::SettingData tmp{param_id,new_value};
            updateData(std::nullopt,tmp);
            return true;
        }
    }
    return false;
}

bool MavlinkSettingsModel::try_update_parameter(const QString param_id,QVariant value)
{
    qDebug()<<"try_update_parameter:"<<param_id<<","<<value;
    int row=0;
    for(const auto& parameter: m_data){
        if(parameter.unique_id==param_id){
            // we got this parameter
            if(param_client){
                if(value.canConvert(QVariant::Type::Int)){
                    int value_int=value.toInt();
                    qDebug()<<"Set"<<param_id<<" to "<<value_int;
                    const auto result=param_client->set_param_int(param_id.toStdString(),value_int);
                    if(result==mavsdk::Param::Result::Success){
                        MavlinkSettingsModel::SettingData tmp{param_id,value_int};
                        updateData(std::nullopt,tmp);
                        return true;
                    }else{
                        std::stringstream ss;
                        ss<<"Updating "<<param_id.toStdString()<<" to "<<value_int<<" failed: "<<result;
                        qDebug()<<QString(ss.str().c_str());
                        makePopupMessage(ss.str().c_str());
                        return false;
                    }
                }
            }
        }
    }
    return false;
}

int MavlinkSettingsModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_data.count();
}

QVariant MavlinkSettingsModel::data(const QModelIndex &index, int role) const
{
    if ( !index.isValid() )
        return QVariant();

    const auto& data = m_data.at(index.row());
    if ( role == UniqueIdRole ){
        return data.unique_id;
    }
    else if ( role == ValueRole )
        return data.value;
    else
        return QVariant();
}

QHash<int, QByteArray> MavlinkSettingsModel::roleNames() const
{
    static QHash<int, QByteArray> mapping {
        {UniqueIdRole, "unique_id"},
        {ValueRole, "value"}
    };
    return mapping;
}


void MavlinkSettingsModel::removeData(int row)
{
    if (row < 0 || row >= m_data.count())
        return;

    beginRemoveRows(QModelIndex(), row, row);
    m_data.removeAt(row);
    endRemoveRows();
}

void MavlinkSettingsModel::updateData(std::optional<int> row_opt, SettingData new_data)
{
    int row=-1;
    if(row_opt.has_value()){
        row=row_opt.value();
    }else{
        for(int i=0;i<m_data.size();i++){
            if(m_data.at(i).unique_id==new_data.unique_id){
                row=i;
                break;
            }
        }
    }
    if (row < 0 || row >= m_data.count())
        return;
    if(m_data.at(row).unique_id!=new_data.unique_id){
        qDebug()<<"Argh";
        return;
    }
    m_data[row]=new_data;
    QModelIndex topLeft = createIndex(row,0);
    emit dataChanged(topLeft, topLeft);
}

void MavlinkSettingsModel::addData(MavlinkSettingsModel::SettingData data)
{
    if(is_param_whitelisted(data.unique_id.toStdString())){
        // never add whitelisted params to the simple model, they need synchronization
        return;
    }
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_data.push_back(data);
    endInsertRows();
}
