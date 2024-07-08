#include <LiquidCrystal_I2C.h> // I2C ekranlarının kontrol edilmesine olanak tanımaktadır.
#include <Wire.h> // I2C cihazları ile iletişimi sağlamaktadır.
#include "thingProperties.h" // ArduinoIoTCloud bağlantı dosyamı aktif ettim.
#include "DHT.h" // DHT22 sensörü için kütüphane bağlantısını yaptım.

#define ledPin 12 // LED'i bağladığım pin.
#define DHTPIN 0 // DHT sensörünün bağlı olduğu pin.
#define DHTTYPE DHT22 // DHTTYPE yerinde DHT22 tanımlaması gerçekleştirdim.

const int I2C_ADDR = 0x27; // I2C adresi.
const int LCD_COLS = 16;   // LCD sütun sayısı.
const int LCD_ROWS = 2;    // LCD satır sayısı.
LiquidCrystal_I2C lcd=LiquidCrystal_I2C(I2C_ADDR, LCD_COLS, LCD_ROWS); // LCD pinlerini ayarlama işlemi gerçekleştirdim.

DHT dht(DHTPIN, DHTTYPE); // DHT22 sensörü ile iletişimi gerçekleştirdim.

int x=0; // LCD açıldığında DHT22 sensöründe bulunan sıcaklık ve nem değerlerini ArduinoCloud'a aktarmak için kullanıyorum.
// Bunu kullanmamın nedeni ise cloud ortamına bağlandığımda DHT22 sensörü sıcaklık ve nem değerleri ile oynama yapmayıp otomatik olarak yani default değer vermek için kullandım.

unsigned long onceki_zaman = 0; // Önceki zamanı saklamak için değişkendir.
const long saniye = 2000; // İşlemler arasındaki süre (ms).

int y=0;// RR'yi kontrol etmek için kullandım.

void setup() {

  Serial.begin(9600);// Seri iletişimi başlatmak için kullandım. 9600 değeri ise saniye başına gönderilen ve alınan bit anlamına gelmektedir.
  delay(1000);// 1 saniye beklettim.
  pinMode(ledPin,OUTPUT); // Led pininmi çıkış olarak ayarladım.

  // Lcd ekranını başlat
  dht.begin(); // DHT sensörü nesnesini başlattım.
  lcd.init(); // LCD nesnesini başlattım.
  lcd.backlight(); // Aydınlatma biçimi sağladım.
  lcd.setCursor(0,0); // 1. sütun, 1. satıra belirledim.
  lcd.print("LCD INITIALIZING"); // LCD başlatıldığında "LCD BAŞLATILIYOR" yazısını ekledim. 
  delay(1000); // 1. saniye beklettim.
  lcd.clear(); // LCD ekranını temizledim.
  delay(1000); // 1. saniye beklettim.
  
  initProperties(); // "thingProperties.h" dosyamdaki initProperties() fonksiyonumu çağırdım ve cloud ortamına bağlantı kurmak için kullandım.
  ArduinoCloud.begin(ArduinoIoTPreferredConnection); // ArduinoCloud ortamını başlattım. Daha sonrasında tercih ettiğim bağlantımı içersine yerleştirdim.
  // Ödevde WiFi bağlantısı istendiği için "thingProperties.h" dosyamdan dönen WiFi bağlantılı nesnemi içersine yerleştirdim.
  setDebugMessageLevel(2); // Hata ayıklama aracımı belirledim. 2 olarak da hata yakalama seviyemi belirledim.
  ArduinoCloud.printDebugInfo(); // Hata ayıklama bilgilerini yazdırmak için kullanılır.
  Serial.println("Bağlandı!"); // Terminal ekranına "Bağlandı!" mesajını yazdırdım.
}

void loop() {
  if(x==0)
  {
      // ArduinoIotCloud'da dashboard yapısında hiçbir veri gözükmeyip DHT22 sensöründen veriler ile oynamak yerine default değer atama işlemi gerçekleştirildi.
      sicaklik = dht.readTemperature(); // DHT22 sensöründen sıcaklık bilgisini çektim.
      nem = dht.readHumidity(); // DHT22 sensöründen nem bilgisini çektim.
      x+=1; // Default olarak cloudda veri gözüksün diye x değişkenini arttırdım.
      // Bu kısmı 1 kere kullandım.
  }

  unsigned long gecerli_zaman = millis(); // Geçerli zaman aldım.

  if (gecerli_zaman - onceki_zaman >= saniye) { // 2 saniye geçti mi kontrolü?
    onceki_zaman = gecerli_zaman; // Zamanı güncellendim.

    if (y == 0) {
      dhtNemOkuma(); // Nem okuma işlemini gerçekleştirdim.
    } else if (y == 1) {
      dhtSicaklikOkuma(); // Sıcaklık okuma işlemimini gerçekleştirdim.
    } else if (y == 2) {
        ArduinoCloud.update(); // Arduino Cloud güncelleme işlemi yaptım.               
    }

    y+=1; // Sonraki işleme geçmek için y değeri 1 arttırıldı.

    // y değeri 3 olduğunda sıfırlandı ve RR yeniden başlatıldı.
    if (y == 3) {
      y = 0; // İlk işleme dönüldü.
    }
  }
}

void onLedChange()  {
  if(led == true || dht.readTemperature() >=25) // Led'in yüksek sıcaklıklarda veya switch'den kontrolü için şart kısmını yazdım.
  {
    digitalWrite(ledPin, HIGH); // Led'i yaktım.
  }
  else{
    digitalWrite(ledPin, LOW); // Led'i söndürdüm.
  }
}


void onMesajChange()  {
  
  if(mesaj!="")
  {
    mesaj.trim(); // Mesaj boşluklu geliyordu onu engelledim.
    lcd.clear(); // Lcd ekranını temizledim.
    lcd.setCursor(0, 0); // 1.sütun, 1.satır veri yazacağımı belirttim.
    lcd.print(mesaj); // Lcd ekranıma mesaj yazdım. Bu mesaj cloud ortamından gelen verileri göstermek için kullanılıyor.
  }

}


void dhtNemOkuma(){
  lcd.clear(); // Lcd ekranımı temizledim.
  float humidity = dht.readHumidity(); // DHT22 sensöründen nem verimi çektim.
  nem=humidity; // Clouddaki değişkenime  "nem" değikenimi gönderdim.

  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print("%"); // Terminale yazma işleminde println kullanmadım. Çünkü sıcaklık bilgisi geldikten sonra bir sonraki satıra geçmesi gerekiyor.
  // Yukarıdaki gibi bir mantık ile terminale yazma işlemi gerçekleştirdim.

  if (humidity <= 50) { 
    lcd.setCursor(4, 0); // 5.sütun, 1.satır olarak lcd ekranımı ayarladım.
    lcd.print("Humidity"); // Lcd ekranıma "Nem" yazdırdım.
    lcd.setCursor(3, 1); // 4.sütun, 2.satır olarak lcd ekranımı ayarladım.
    lcd.print(humidity); // Lcd ekranına nem bilgisini yazdım.
    lcd.setCursor(8, 1); // 9.sütun, 2.satır olarak lcd ekranımı ayarladım.
    lcd.print("-LOW"); // Nem değeri 50 ve altı olduğu için "Düşük" olarak lcd ekranına yazdırdım.
  } 
  else {
    lcd.setCursor(4, 0); // 5.sütun, 1.satıra lcd ekranımı ayarladım.
    lcd.print("Humidity"); // Lcd ekranıma "Nem" stringi yazdırdım.
    lcd.setCursor(3, 1); // 4.sütun, 2.satıra lcd ekranımı ayarladım.
    lcd.print(humidity); // Lcd ekranıma "Nem" değerimi yazdırdım.
    lcd.setCursor(8, 1); // 9.sütun, 2.satıra lcd ekranımı ayarladım.
    lcd.print("-HIGH"); // Lcd ekranıma "Nem" değerim 50 üstü olduğu için "Yüksek" yazdırdım.
  }

}


void dhtSicaklikOkuma(){

  lcd.clear(); // Lcd ekranımı temizledim.
  
  float temperature = dht.readTemperature(); // DHT22 sensöründen sıcaklık verimi çektim.
  sicaklik=temperature; // Clouddaki değişkenime  "sicaklik" değikenimi gönderdim.


  // Aşağıda iot ödev raporunda belirtilen şekilde verileri terminale yazdım.
  // Aşağıda println kullandıktan sonra bir alt satıra geçeceği için proje isterlerindeki terminal çıktısı terminale basılıyor.

  Serial.print(" || ");
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println("°C");

  if (temperature < 0) {
    lcd.setCursor(2, 0); // 3.sütun, 1.satıra lcd ekranımı ayarladım.
    lcd.print("Temperature"); // Lcd ekranıma "Sıcaklık" stringi yazdırdım.
    lcd.setCursor(0, 1); // 1.sütun, 2.satıra lcd ekranımı ayarladım.
    lcd.print(temperature); // Lcd ekranıma "Sıcaklık" değerimi yazdırdım.
    lcd.setCursor(6, 1); // 7.sütun, 2.satıra lcd ekranımı ayarladım.
    lcd.print("-VERY LOW"); // Lcd ekranıma "Sıcaklık" değerim 0'ın altında olduğu için "Çok düşük" yazdırdım.
    digitalWrite(ledPin, LOW); // LED'i söndürdüm.
  } 
  else if (temperature <= 25) {

    lcd.setCursor(2, 0); // 3.sütun, 1.satıra lcd ekranımı ayarladım.
    lcd.print("Temperature"); // Lcd ekranıma "Sıcaklık" stringi yazdırdım.
    lcd.setCursor(3, 1); // 4.sütun, 2.satıra lcd ekranımı ayarladım.
    lcd.print(temperature); // Lcd ekranıma "Sıcaklık" değerimi yazdırdım.
    lcd.setCursor(8, 1); // 9.sütun, 2.satıra lcd ekranımı ayarladım.
    lcd.print("-NORMAL"); // Lcd ekranıma "Sıcaklık" değerim 0 ve 25 arasında olduğu için "Normal" yazdırdım.
    digitalWrite(ledPin, LOW); // LED'i söndürdüm.
  } 
  else {
    lcd.setCursor(2, 0); // 3.sütun, 1.satıra lcd ekranımı ayarladım.
    lcd.print("Temperature"); // Lcd ekranıma "Sıcaklık" stringi yazdırdım.
    lcd.setCursor(3, 1); // 4.sütun, 2.satıra lcd ekranımı ayarladım.
    lcd.print(temperature); // Lcd ekranıma "Sıcaklık" değerimi yazdırdım.
    lcd.setCursor(8, 1); // 9.sütun, 2.satıra lcd ekranımı ayarladım.
    lcd.print("-HIGH"); // Lcd ekranıma "Sıcaklık" değerim 25 üzerinde olduğu için "Normal" yazdırdım.
    digitalWrite(ledPin, HIGH); // LED'i yaktım.
  }
}