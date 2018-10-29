
#include <SPI.h>
#include <MFRC522.h>
#define SS_PIN 10
#define RST_PIN 7
char task;
MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class
MFRC522::MIFARE_Key key;
int s1=0;
int s2=0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522
  //rfid.PCD_SetAntennaGain(rfid.RxGain_max);


}
void loop() {

task='0';
  if (Serial.available()) {
    task = Serial.read();
  }
    
    if (task == '1') {
      int s;
      s1=1;
      while(s1!=0){
        String ab = reuid();
        if (ab != "null")
            {Serial.println(ab+"\n");
            //input screen driver code here
            }
        if (Serial.available())  
            {s = Serial.read();}
        if (s=='!')
            {s1=0;}
            }}

      
    if (task == '2') {
      int s;
      s2=1;
      while(s2!=0){
        amount();
        if (Serial.available())  
            {s = Serial.read();}
        if (s=='!')
            {s2=0;}
            }}
    
    if (task == '3') {wri();}
    if (task =='4'){Serial.println("available");}

delay(1000);
  }





  //////////////////////read functions///////////////
 String reuid() {
    String res = "null";
    // put your main code here, to run repeatedly:
    if ( ! rfid.PICC_IsNewCardPresent())
      return res;
    // Verify if the NUID has been readed
    if ( ! rfid.PICC_ReadCardSerial())
      return res;
    MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);//get the type
    res = printHextxt(rfid.uid.uidByte, rfid.uid.size);
   // Serial.println(' ');
    // Halt PICC
    rfid.PICC_HaltA();
    // Stop encryption on PCD
    rfid.PCD_StopCrypto1();
  }


  String printHextxt(byte * buffer, byte bufferSize) {
    String coa = "";
    for (byte i = 0; i < bufferSize; i++) {
      String a = String(buffer[i], DEC);
      coa = coa + a;
    }
    return coa;
  }
  /////////////////////////////////////////////////test amount///////////////////////////
  void amount() {

    MFRC522::MIFARE_Key key;
    for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;

    MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance

    //some variables we need
    byte block;
    byte len;
    MFRC522::StatusCode status;

    //-------------------------------------------

    // Look for new cards
    if ( ! mfrc522.PICC_IsNewCardPresent()) {
      return;
    }

    // Select one of the cards
    if ( ! mfrc522.PICC_ReadCardSerial()) {
      return;
    }

    //Serial.println(F("**Card Detected:**"));

    //-------------------------------------------

    // mfrc522.PICC_DumpDetailsToSerial(&(mfrc522.uid)); //dump some details about the card
    //-------------------------------------------

    //Serial.print(F("Name: "));

    byte buffer1[18];

    block = 4;
    len = 18;

    //------------------------------------------- GET FIRST NAME
    status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, 4, &key, &(mfrc522.uid)); //line 834 of MFRC522.cpp file
    if (status != MFRC522::STATUS_OK) {
      Serial.print(F("Authentication failed: "));
      Serial.println(mfrc522.GetStatusCodeName(status));
      return;
    }

    status = mfrc522.MIFARE_Read(block, buffer1, &len);
    if (status != MFRC522::STATUS_OK) {
      Serial.print(F("Reading failed: "));
      Serial.println(mfrc522.GetStatusCodeName(status));
      return;
    }

    //PRINT FIRST NAME
    for (uint8_t i = 0; i < 16; i++)
    {
      if (buffer1[i] != 32)
      {
        Serial.write(buffer1[i]);
      }
    }
    Serial.print(" ");

    //---------------------------------------- GET LAST NAME

    byte buffer2[18];
    block = 1;

    status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, 1, &key, &(mfrc522.uid)); //line 834
    if (status != MFRC522::STATUS_OK) {
      Serial.print(F("Authentication failed: "));
      Serial.println(mfrc522.GetStatusCodeName(status));
      return;
    }

    status = mfrc522.MIFARE_Read(block, buffer2, &len);
    if (status != MFRC522::STATUS_OK) {
      Serial.print(F("Reading failed: "));
      Serial.println(mfrc522.GetStatusCodeName(status));
      return;
    }
    for (uint8_t i = 0; i < 16; i++) {
      Serial.write(buffer2[i] );
    }
    Serial.print("\n");
    delay(1000); //change value if you want to read cards faster
    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();
  }

  /////////////////////////write///////////////////////////////////

  void wri() {

    // Prepare key - all keys are set to FFFFFFFFFFFFh at chip delivery from the factory.
    MFRC522::MIFARE_Key key;
    for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;
    MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance

    // Look for new cards
    if ( ! mfrc522.PICC_IsNewCardPresent()) {
      return;
    }

    // Select one of the cards
    if ( ! mfrc522.PICC_ReadCardSerial()) {
      return;
    }

    //  Serial.print(F("Card UID:"));    //Dump UID
    //  for (byte i = 0; i < mfrc522.uid.size; i++) {
    //    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    //    Serial.print(mfrc522.uid.uidByte[i], HEX);
    //  }
    //Serial.print(F(" PICC type: "));   // Dump PICC type
    MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
    // Serial.println(mfrc522.PICC_GetTypeName(piccType));

    byte buffer[34];
    byte block;
    MFRC522::StatusCode status;
    byte len;

    Serial.setTimeout(20000L) ;     // wait until 20 seconds for input from serial
    // Ask personal data: Family name
    Serial.println(F("Type amount, ending with #\n"));
    len = Serial.readBytesUntil('#', (char *) buffer, 30) ; // read family name from serial
    for (byte i = len; i < 30; i++) buffer[i] = ' ';     // pad with spaces

    block = 1;
    //Serial.println(F("Authenticating using key A..."));
    status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(mfrc522.uid));
    if (status != MFRC522::STATUS_OK) {
      Serial.print(F("PCD_Authenticate() failed: "));
      Serial.println(mfrc522.GetStatusCodeName(status));
      return;
    }
    else Serial.println(F("PCD_Authenticate() success: "));

    // Write block
    status = mfrc522.MIFARE_Write(block, buffer, 16);
    if (status != MFRC522::STATUS_OK) {
      Serial.print(F("MIFARE_Write() failed: "));
      Serial.println(mfrc522.GetStatusCodeName(status));
      return;
    }
    else Serial.println(F("MIFARE_Write() success: "));

    block = 2;
    //Serial.println(F("Authenticating using key A..."));
    status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(mfrc522.uid));
    if (status != MFRC522::STATUS_OK) {
      Serial.print(F("PCD_Authenticate() failed: "));
      Serial.println(mfrc522.GetStatusCodeName(status));
      return;
    }

    // Write block
    status = mfrc522.MIFARE_Write(block, &buffer[16], 16);
    if (status != MFRC522::STATUS_OK) {
      Serial.print(F("MIFARE_Write() failed: "));
      Serial.println(mfrc522.GetStatusCodeName(status));
      return;
    }
    else Serial.println(F("MIFARE_Write() success: "));

    // Ask personal data: First name
    Serial.println(F("Type password, ending with \n#"));
    len = Serial.readBytesUntil('#', (char *) buffer, 20) ; // read first name from serial
    for (byte i = len; i < 20; i++) buffer[i] = ' ';     // pad with spaces

    block = 4;
    //Serial.println(F("Authenticating using key A..."));
    status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(mfrc522.uid));
    if (status != MFRC522::STATUS_OK) {
      Serial.print(F("PCD_Authenticate() failed: \n"));
      Serial.println(mfrc522.GetStatusCodeName(status));
      return;
    }

    // Write block
    status = mfrc522.MIFARE_Write(block, buffer, 16);
    if (status != MFRC522::STATUS_OK) {
      Serial.print(F("MIFARE_Write() failed: "));
      Serial.println(mfrc522.GetStatusCodeName(status));
      return;
    }
    else Serial.println(F("MIFARE_Write() success: "));

    block = 5;
    //Serial.println(F("Authenticating using key A..."));
    status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(mfrc522.uid));
    if (status != MFRC522::STATUS_OK) {
      Serial.print(F("PCD_Authenticate() failed: "));
      Serial.println(mfrc522.GetStatusCodeName(status));
      return;
    }

    // Write block
    status = mfrc522.MIFARE_Write(block, &buffer[16], 16);
    if (status != MFRC522::STATUS_OK) {
      Serial.print(F("MIFARE_Write() failed: "));
      Serial.println(mfrc522.GetStatusCodeName(status));
      return;
    }
    else Serial.println(F("MIFARE_Write() success: "));


    Serial.println(" ");
    mfrc522.PICC_HaltA(); // Halt PICC
    mfrc522.PCD_StopCrypto1();  // Stop encryption on PCD

  }
