#!/bin/bash
PORT=/dev/ttyACM0
FILE=/home/mandu/upload_esp32/firmware.bin
esptool.py --chip esp32s3 --port $PORT --baud 115200 --after hard_reset write_flash -z 0x10000 $FILE

# 2. 업로드 성공 확인 및 모니터링 시작
if [ $? -eq 0 ]; then
    echo "업로드 성공! 모니터링을 시작합니다..."
    # minicom을 사용한 모니터링 (115200 보드레이트 설정)
    # -D: 포트 지정, -b: 보드레이트, -C: 로그파일 생성(선택사항)
    picocom /dev/ttyACM0
else
    echo "업로드 실패. 모니터링을 건너뜁니다."
fi