#include "dht11.h"

uint8_t dht_get_tem_humi (uint8_t *tem, uint8_t *humi)
{
	uint8_t buffer[5] = {0};
	uint8_t j,i,checksum;
	DHT11_TRIS = 0;   // set la cong ra
	DHT11_DATA = 1;
    delay_us(60);
    DHT11_DATA = 0;
    delay_ms(20); // it nhat 18ms
    DHT11_DATA = 1;
    //delay_us(40); // doi DHT dap ung trong vong 20us-40us
	DHT11_TRIS = 1;
	delay_us(60);
    if(DHT11_DATA)
		return DHT_ER ;
	else
		while(!DHT11_DATA);	//Doi DaTa len 1

    delay_us(60);
    if(!DHT11_DATA)
		return DHT_ER;
	else
		while((DHT11_DATA));	 //Doi Data ve 0
	//Bat dau doc du lieu
	for(i = 0; i < 5; i++)
	{
		for(j = 0; j < 8; j++)
			{
				while(!DHT11_DATA);//Doi Data len 1
				delay_us(50);
				if(DHT11_DATA)
				{
					buffer[i] |= (1 << (7 - j));
					while((DHT11_DATA));//Doi Data xuong 0
				}
			}
	}

	checksum = buffer[0] + buffer[1] + buffer[2] + buffer[3];
    if(checksum != buffer[4])
		return DHT_ER;
   	*tem  =  buffer[2];
	*humi =  buffer[0];
	return DHT_OK;
}
