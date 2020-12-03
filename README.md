# **ロボットシステム学HW1**  
- デバイスドライバを使用してLEDを点滅させ、モールス信号を表現する。
***
## **使用する道具**  
- Raspberry Pi 4 model B  
- LED  
- ブレッドボード  
- 抵抗（220Ω）  
- ジャンパー線  
***
## **環境**  
- Ubuntu 18.04 LTS  
***
## **配線**  
- GPIO 25とGNDに接続する。  
***
## **実行方法**  
- 以下のコマンドを上から順に実行する。  
```
$ git clone https://github.com/kazukishirasu/robotsystem_HW1.git  
$ make  
$ sudo rmmod myled  
$ sudo insmod myled.ko  
$ sudo chmod 666 /dev/myled0  
$ echo 1 > /dev/myled0  
```  
***
## **実行結果**  
- 今回は「**－・－　・－　－－・・　・・－　－・－　・・**（KAZUKI）」というモールス信号を表現した。

- [実行時の動画](https://youtu.be/-zq_EfpfOzQ)  
