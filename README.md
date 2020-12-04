# **ロボットシステム学HW1**  
- デバイスドライバを使用してLEDを点滅させ、モールス信号を表現する。
***
## **使用する道具**  
- Raspberry Pi 4 model B  
- 赤色LED  
- RGB LED
- ブレッドボード  
- 抵抗
  - 220Ω×4
- ジャンパー線
  -  オス~メス×5
  -  オス~オス×1
***
## **環境**  
- Ubuntu 18.04 LTS  
***
## **配線**  
- GPIO 25を赤色LED、GPIO 24をRGB LEDの赤色、GPIO 23を緑色、GPIO 22を青色、GNDに接続する。  
***
## **実行方法**  
- 以下のコマンドを上から順に実行する。  
```
$ git clone https://github.com/kazukishirasu/robotsystem_HW1.git 
$ cd robotsystem_HW1
$ make  
$ sudo rmmod myled  
$ sudo insmod myled.ko  
$ sudo chmod 666 /dev/myled0  
$ echo 1 > /dev/myled0  
```
***
## **実行結果**  
-  赤・黄色・青で信号機を表現した。
-  赤色LEDで「**－・－　・－　－－・・　・・－　－・－　・・**（KAZUKI）」というモールス信号を表現した。

- [実行時の動画](https://youtu.be/JRdC526gu3k)  
***
## **参考資料**
モールス信号(https://images.app.goo.gl/aD6JNsrwYYHWkuCFA)  
***
共同開発者・・・髙橋祐樹、中島勇
