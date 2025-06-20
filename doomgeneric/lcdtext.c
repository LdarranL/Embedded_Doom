#include "lcdtext.h"
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>

stTextLCD stlcd;
static int fd = -1;

int txtlcd_Init(void)
{
    fd = open("/dev/peritextlcd", O_RDWR);
    if (fd < 0)
    {
        perror("driver (//dev//peritextlcd) open error.\n");
        return 1;
    }
    memset(&stlcd, 0, sizeof(stTextLCD));
	return 0;
}

void lcdtextwrite(const char *str1, const char *str2, int lineFlag)
{
    if (fd < 0) return;  // 초기화 실패 시 무시

    // 1) 명령 구조체 초기화
    memset(&stlcd, 0, sizeof(stTextLCD));
    stlcd.cmd = CMD_WRITE_STRING;
 // 줄 지정 및 데이터 복사
    if (lineFlag == 1)
    {
        stlcd.cmdData = CMD_DATA_WRITE_LINE_1;
        if (str1)
            strncpy(stlcd.TextData[0], str1, COLUMN_NUM);
    }
    else if (lineFlag == 2)
    {
        stlcd.cmdData = CMD_DATA_WRITE_LINE_2;
        if (str2)
            strncpy(stlcd.TextData[1], str2, COLUMN_NUM);
    }
    else // both lines
    {
        stlcd.cmdData = CMD_DATA_WRITE_BOTH_LINE;
        if (str1)
            strncpy(stlcd.TextData[0], str1, COLUMN_NUM);
        if (str2)
            strncpy(stlcd.TextData[1], str2, COLUMN_NUM);
    }

    // 3) 드라이버로 전송
    //sleep(1);
    if (write(fd, &stlcd, sizeof(stTextLCD)) < 0) {
        perror("lcdtextwrite: write error");
    }
}

void txtlcd_off(void)
{
    char blank[COLUMN_NUM+1] = {0};
    lcdtextwrite(blank,NULL, 1);
    lcdtextwrite(blank,NULL, 2);
    close(fd);
}
