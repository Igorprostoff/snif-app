# Приложение-снифер WiFi трафика

## Функционал
Приложение осуществляет перхват WiFi фреймов и вывод информации о перехваченных фреймах, в частности, MAC-адрес отправителя, MAC-адрес получателя, MAC-адрес 
беспроводной точки доступа, уровень сигнала и номер канала. Вывод информации осуществляется на дисплей устройства и также по СOM-порту.
Одновременно возможно прослушивание одного канала WiFi. Переключение канала осуществляется путем подачи логического нуля на интерфейс GPIO_0.

## Аппаратная база
В качестве аппаратной базы приложение использует микроконтроллер ESP32.

## Требования к программному обеспечению
Для сборки кода и прошивки микроконтроллера требуется установить ПО ESP-IDF версии 4.4 (либо выше) а также систему сборки CMake.

## Процесс сборки и установки
После установки необходимого ПО требуется перейти в каталог приложения и ввести следующую команду

    idf.py menuconfig
    
В открывшемся меню требуется выбрать пункт Project Configuration и указать в нем параметры работы дисплея (интерфейс подключения и используемые GPIO контакты). 
Пример настройки:
![config-example](https://i.ibb.co/g6T3tb9/example-config.png)

После настройки экрана требуется выйти из меню, сохранив изменения и ввести следующую команду:


    idf.py -p /dev/tty.usbserial-0001 flash monitor 
    
где:
- idf.py - установленная утилита для работы с микроконтроллерами ESP
- -p /dev/tty.usbserial-0001 - флаг, указывающий на COM-порт, к которому подключена плата (в Windows может быть обозначен как COM1, COM2 ...)
- flash - указание собрать исходный код и записать его во флеш-память устройства
- monitor - указание подключиться к устройству для мониторинга вывода

После этого устройство готово к работе, переключение каналов в случае с ESP32 осуществляется нажатием кнопки Boot.

На экране будет отображаться следующая информация (по номерам строк)
1. Адрес получателя
2. Адрес отправителя
3. Адрес беспроводной точки доступа
4. Уровень сигнала
5. Номер канала (также уведомление об изменении канала)

### Авторы
- Простов И.А.
- Амфитеатрова С.С.
- Проект https://github.com/ESP-EOS/ESP32-WiFi-Sniffer
