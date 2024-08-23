# DemoApp BLE Single Model version

LASSDK_DEMO/LASSDK/firmware/src/apps



<!-- # DEMO -->
# Add or modify files
Add
* LASSDK\firmware\src\apps\sensor\src\BLEApp.cc
* LASSDK\firmware\src\apps\sensor\src\BLEApp-priv.h

Modify
* LASSDK\firmware\src\apps\sensor\src\SensorApp-priv.h
* LASSDK\firmware\src\apps\sensor\src\SensorAppActionDetection.cc
* LASSDK\firmware\src\config\device\cxd32xx-nuttx_device\bleabstract.mk
* LASSDK\firmware\nv-data\sfp-settings\sfpparam.conf


# Features
BLE(GATT)でPC(Dongle)またはAndroidにDNN検出結果を送信するためのapps (TouchPadでアクション検出モデルとヘッドジェスチャー検出モデルの切り替えが可能)

# Requirement

* SDK version ≧ 1

<!-- # Installation

```bash
None
``` -->

# Usage
既存のappsを置き換える
```bash
cd LASSDK/firmware/src
git clone https://github.com/Kenji-Horiuchi/LASSDK_DEMO_BLE_SingleModelVersion.git
rm -r ./apps
cp -r ./LASSDK_DEMO_BLE_SingleModelVersion/apps ./
```

# Note

apps以外にも以下の個所を変更する必要があることに注意
* LASSDK_DEMO\LASSDK\firmware\nv-data\sfp-settings\sfpparam.conf
* LASSDK_DEMO\LASSDK\firmware\src\config\device\cxd32xx-nuttx_device\bleabstract.mk

<!-- # Author

* 作成者
* 所属
* E-mail

# License
ライセンスを明示する

"hoge" is under [MIT license](https://en.wikipedia.org/wiki/MIT_License).

社内向けなら社外秘であることを明示してる

"hoge" is Confidential. -->