//
// Copyright 2023 Sony Semiconductor Solutions Corporation.
//

// #include <cstdio>
#include <thread>
#include <chrono>
#include <deque>
#include "AsyncTask.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <fcntl.h>
#include "GetManager.h"
#include "BLEApp-priv.h"

#define NUMBER_OF_ACTION 4

namespace apps {
  namespace sensor {

    
    // ***** （済）宣言部分はヘッダーに配置して、定義部分はこのファイルに配置する *****
    ConvertDetectionResultToUint8::ConvertDetectionResultToUint8(std::string InputName, std::vector<uint8_t> InputData){ // コントラクタ
        Name = InputName;
        Data = InputData;
    }
    std::string ConvertDetectionResultToUint8::GetName(){
        return Name; // this->Name;
    }
    std::vector<uint8_t> ConvertDetectionResultToUint8::GetData(){
        return Data; // this->Data;
    }
    ConvertDetectionResultToUint8::~ConvertDetectionResultToUint8(){ // デストラクタ
        // 必要な場合は処理を記述する
    }

    GATT_DATA::GATT_DATA(std::vector<uint8_t> data, uint8_t id, uint16_t data_size){ // コンストラクタ
        send_data = data;
        connection_id = id;
        size = data_size;
    }
    // 送信データ変更
    void GATT_DATA::SendData_StrToUint8(std::string str, uint8_t id, uint16_t data_size){
        ConvertDetectionResultToUint8 convert_table[] = {
            {"STABLE",  {0}},
            {"WALKING", {1}},
            {"RUNNING", {2}},
            {"OTHERS", {200}},
        }; // 4要素あるので、4回コンストラクタが呼ばれている
        
        std::string Name;
        std::vector<uint8_t> Data;

        for(int num=0; num<NUMBER_OF_ACTION; num++){
            Name = convert_table[num].GetName();
            Data = convert_table[num].GetData();
            // auto test = convert_table[num];
            
            if(str == Name){ // コンボリストの中に一致するコンボはあるか
                send_data = Data;
                // std::cout << "before: " << str;
                printf("before: %d\n", str);
                printf(" -> after: %d\n", *send_data.data());
                break;
            }
        }
        connection_id = id;
        size = data_size;
    }
    uint8_t* GATT_DATA::GetData(){
        return send_data.data();
    }
    uint8_t GATT_DATA::GetID(){
        return connection_id;
    }
    uint16_t GATT_DATA::GetSIZE(){
        return size;
    }
    std::vector<uint8_t>* GATT_DATA::GetData_vector(){ // 戻り値をベクター型にする場合
        return &send_data; // .data(); // &send_dataはベクター全体の先頭アドレスとして認識されるが、.data()は先頭のuint8_tのアドレスと認識されるからエラーになるのかも
    }
    GATT_DATA::~GATT_DATA(){ // デストラクタ
        // 必要な場合は処理を記述する
    }

    // // 今回（下から移動）
    // std::vector<uint8_t> send_data(1, 100); // 初期化時にデータを格納しないとうまく動かない // std::vector<uint8_t> send_data;
    // uint8_t connection_id = 1;
    // uint16_t size = 1;
    // // std::string ClassificationStr = "";
    // // GATT_DATA gatt_dataset(send_data, connection_id, size); // 初期化

  }  // namespace sensor
}  // namespace apps