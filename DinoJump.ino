#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

const int buttonPin = 2; 
const int buzzerPin = 9; 
int score = 0; 
int jumpHeight = 0;  
bool isJumping = false; 

// Pola karakter kelinci
byte kelinci[8] = {
  0b00000,
  0b00101,
  0b00101,
  0b00111,
  0b01111,
  0b11111,
  0b10010,
  0b11011,
};

// Pola karakter batu menempel di bawah
byte batu[8] = {
  0b00000,
  0b00000,
  0b00100,
  0b01110,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
};

bool gameOver = false;  

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);  
  pinMode(buzzerPin, OUTPUT);      
  
  lcd.init();
  lcd.backlight();
  lcd.createChar(0, kelinci);    // Buat karakter kelinci
  lcd.createChar(1, batu);       // Buat karakter batu

  Serial.begin(9600); 
}

void loop() {
  // Cek apakah permainan berakhir
  if (gameOver) {
    lcd.clear();
    
    // Animasi untuk tampilan "Loser"
    for (int i = 0; i < 3; i++) {
      lcd.setCursor(4, 0);    // Tampilkan "Loser" di baris pertama
      lcd.print("*Loser*");
      delay(300);
      lcd.clear();
      delay(300);
    }
    
    lcd.setCursor(4, 0);    
    lcd.print(">> Loser <<");  // Teks akhir yang lebih menarik
    
    // Tampilkan skor dengan bingkai sederhana
    lcd.setCursor(2, 1);
    lcd.print("----------------");
    lcd.setCursor(5, 1);
    lcd.print("Score: ");
    lcd.print(score);
    lcd.setCursor(13, 1);
    lcd.print("-");

    // Buzzer berbunyi saat game over
    tone(buzzerPin, 1000, 500); // Suara buzzer dengan frekuensi 1000 Hz selama 500 ms

    // Tunggu hingga tombol ditekan untuk memulai ulang
    while (digitalRead(buttonPin) == HIGH) {
      // Menunggu tombol ditekan
    }
    
    // Reset game dan skor
    score = 0;
    gameOver = false;
    jumpHeight = 0; 
    delay(500); // Tambahkan sedikit jeda setelah reset
  } else {
    // loop batu bergerak
    for (int position = 15; position >= 0; position--) {
      lcd.clear(); 

      // Cek jika tombol ditekan untuk membuat kelinci melompat
      if (digitalRead(buttonPin) == LOW && !isJumping) {
        isJumping = true; 
        tone(buzzerPin, 1000, 200); 
      }

      // Logika lompat max 3
      if (isJumping) {
        if (jumpHeight < 3) {
          lcd.setCursor(3, 0);  
          lcd.write((byte)0);   // Tampilkan kelinci di baris pertama saat melompat
          jumpHeight++;    
        } else {
          // Setelah mencapai puncak, turunkan kelinci ke bawah
          lcd.setCursor(3, 1);   
          lcd.write((byte)0);    // Tampilkan kelinci di baris kedua saat di bawah
          jumpHeight = 0;        
          isJumping = false;     
          noTone(buzzerPin);     
        }
      } else {
        lcd.setCursor(3, 1);   
        lcd.write((byte)0);    // Tampilkan kelinci di baris kedua
      }

      // Jika posisi batu mencapai 4, tambahkan skor
      if(position == 4) {
        score += 1;
      }

      // Tampilkan skor di sudut kanan atas
      lcd.setCursor(15, 0);         
      lcd.print(score);    

      // Tampilkan batu di baris kedua
      lcd.setCursor(position, 1);    
      lcd.write((byte)1);            // Tampilkan batu di bawah

      // Cek tabrakan (game over) jika posisi batu dan kelinci bertemu
      if (position == 3 && !isJumping) {
        gameOver = true;
        break; 
      }

      delay(200); 
    }
  }
}
