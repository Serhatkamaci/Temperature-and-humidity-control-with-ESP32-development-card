Bu çalışmada, ESP32 geliştirme kartı kullanılarak sıcaklık ve nem ölçümlerine bağlı olarak belirli koşullarda LED kontrolü sağlayan bir uygulama geliştirilmiştir. Uygulama, Wokwi simülasyon ortamında gerçekleştirilmiştir. Proje çalıştırma adımları şu şekildedir:

1-) İlgili Wokwi linkine tıklayın.

2-) Wokwi platformunda "IoT projem" adlı projeye erişeceksiniz.

3-) Simülasyon sekmesinde, "Run (Yeşil buton)" düğmesine basarak projeyi derleyin ve LCD ekranın açılmasını bekleyin.

4-) Başlangıçta, "ArduinoIoTCloud" platformunda değerler görünmeyebilir. "ArduinoIoTCloudTCP::handle_WaitDeviceConfig device waiting for valid thing_id" mesajını terminalde görünceye kadar bekleyin.

5-) Eğer "ArduinoIoTCloud" platformunda "Messenger" bileşeninde bir ileti varsa, son ileti LCD ekranında görünecektir. Sensör değerlerini değiştirmek isterseniz, Wokwi simülasyonunda bulunan "DHT22" sensörüne tıklayarak sıcaklık ve nem değerlerini değiştirebilirsiniz.

6-) Proje çalıştırma adımları bu şekildedir. Dinlediğiniz için teşekkür ederim, iyi günler dilerim.

--------------------------------------------------

In this study, an application that provides LED control under certain conditions based on temperature and humidity measurements was developed using the ESP32 development board. The application was carried out in the Wokwi simulation environment. The project running steps are as follows:

1-) Click on the relevant Wokwi link.

2-) You will access the project called "my IoT project" on the Wokwi platform.

3-) In the Simulation tab, compile the project by pressing the "Run (Green button)" button and wait for the LCD screen to open.

4-) Initially, values ​​may not appear on the "ArduinoIoTCloud" platform. Wait until you see the message "ArduinoIoTCloudTCP::handle_WaitDeviceConfig device waiting for valid thing_id" in the terminal.

5-) If there is a message in the "Messenger" component on the "ArduinoIoTCloud" platform, the last message will appear on the LCD screen. If you want to change the sensor values, you can change the temperature and humidity values ​​by clicking on the "DHT22" sensor in the Wokwi simulation.

6-) The steps to run the project are as follows. Thank you for listening and have a nice day.
