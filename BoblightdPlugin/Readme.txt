BoblightdPlugin - плагин, реализующий поддержку ambient-light модуля Paintpack (DMX версии) http://paintpack.ru/ в Boblightd http://code.google.com/p/boblight/

В отличии от Prismatic-а, boblight-X11 обеспечивает быстрый захват изображения, используя всего 7% CPU на одноядерном Intel Atom-е. 
Именно поэтому рекомендуется для использования в системах с ограничеными ресурсами, хотя сборка и настройка занимает определенное время.

Основое предназначение этого мануала - работа в XBMC в целом и OpenELEC.tv в частности.

Порядок установки:

1. Устанавливает libusb и libudev. Ну и инструменты сборки.
В Debian/Ubuntu - apt-get install libusb-1.0.0-dev libudev-dev build-essential
Собирать необходимо на PC с архитектурой и версией libc аналогичными целевой.

2. Клонируем HID API от Signal11 http://www.signal11.us/oss/hidapi/
git clone git://github.com/signal11/hidapi.git

3. Собираем статическую библиотеку
cd hidapi/linux/ (или cd hidapi/mac/)
cp Makefile-manual Makefile
make

4. Берем paintpack.c и скачиваем в текущую директорию

5. Собираем контрольную программу painpack
cp ../hidapi/hidapi.h .
Для Linux: g++ paintpack.c hid.o -ludev -o paintpack
Для OSX: g++ paintpack.c hid.o -framework IOKit -framework CoreFoundation -o paintpack

В случае, если вы собираетесь использовать paintpack с OpenELEC.tv, то сохраните в том числе и /lib/libudev.so.0

Дальнейшие инструкции применимы исключительно к OpenELEC.tv, хотя будут работать в любой Linux системе с установленным boblightd.

6. Устанавливаем boblightd аддон из меню XBMC. 
Копируем paintpack в /storage/.xbmc/addons/service.multimedia.boblightd/bin 
Копируем libudev.so.0 в /storage/.xbmc/addons/service.multimedia.boblightd/lib

7. Не забываем про chmod +x и ./paintpack. Если вы все сделали правильно, то вы должны увидеть текст "painpack device not detected".
Можете подключить девайс, в течении 5 секунд должен появиться текст "painpack device connected", а значит все работает.
Для проверки запустите ./paintpack и скопируйте следующий текст:
1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 
Нажмите Enter, все светодиоды должны загореться белым цветом.

8. Переходим к настройке boblight. 
Редактируем файл /storage/.xbmc/userdata/addon_data/service.multimedia.boblightd/boblight.conf в случае XBMC/OpenELEC.
Или /etc/boblight.conf в случае с обычным линуксом.

9. Редактируем [device] секцию.

[device]
name device1
type popen
output /storage/.xbmc/addons/service.multimedia.boblightd/bin/paintpack
channels 30
interval 20000

Output - полный путь к бинарнику paintpack-а.

10. Путем перебора определяем какой из каналов соотвествует какому светодиоду.

[light]
name            light3
color           red     device1 13 // 13 канал для R
color           green   device1 14 // 14 канал для G
color           blue    device1 15 // 15 канал для B
hscan           0 15
vscan           0 33

Перебор можно опять таки делать с помощью собранной paintpack утилиты, копируя в нее попеременно строки
1 1 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 для первых трех каналов
0 0 0 1 1 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 для вторых трех каналов
И так далее, наблюдая за тем, какая из лент загорится белым цветом.

Пример моего рабочего (3 ленты слева, 3 справа, 4 вверху, пропорциональное разделение, каналы вразброс) конфига находится в boblight.conf

Вот и все, еще можно поиграться с настройками boblight-X11 для сглаживания цветосмены, но это уже как кому нравится.