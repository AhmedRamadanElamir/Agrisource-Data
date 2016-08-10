
#include <math.h>
#include <stdint.h>
#include "sensorlib/hw_bmp180.h"
#include "sensorlib/i2cm_drv.h"
#include "sensorlib/bmp180.h"

#define BMP180_STATE_IDLE      0           // State machine is idle
#define BMP180_STATE_INIT1     1           // Waiting for initialization 1
#define BMP180_STATE_INIT2     2           // Waiting for initialization 2
#define BMP180_STATE_READ      3           // Waiting for read
#define BMP180_STATE_WRITE     4           // Waiting for write
#define BMP180_STATE_RMW       5           // Waiting for read-modify-write
#define BMP180_STATE_REQ_TEMP  6           // Requested temperature
#define BMP180_STATE_WAIT_TEMP 7           // Waiting for temperature ready
#define BMP180_STATE_READ_TEMP 8           // Reading temperature value
#define BMP180_STATE_REQ_PRES  9           // Requested pressure
#define BMP180_STATE_WAIT_PRES 10          // Waiting for pressure ready
#define BMP180_STATE_READ_PRES 11          // Reading pressure value

static void
BMP180Callback(void *pvCallbackData, uint_fast8_t ui8Status)
{
    tBMP180 *psInst;
    uint16_t ui16ReadVerify;

    psInst = pvCallbackData;
    if(ui8Status != I2CM_STATUS_SUCCESS)
    {
        psInst->ui8State = BMP180_STATE_IDLE;
    }

    switch(psInst->ui8State)
    {
        case BMP180_STATE_READ:
        case BMP180_STATE_READ_PRES:
        default:
        {
            psInst->ui8State = BMP180_STATE_IDLE;
            break;
        }

        case BMP180_STATE_INIT1:
        {
            psInst->pui8Data[0] = BMP180_O_AC1_MSB;
            I2CMRead(psInst->psI2CInst, psInst->ui8Addr, psInst->pui8Data, 1,
                     psInst->uCommand.pui8Buffer, 22, BMP180Callback, psInst);

            psInst->ui8State = BMP180_STATE_INIT2;
            break;
        }

        case BMP180_STATE_INIT2:
        {
            ui16ReadVerify = psInst->uCommand.pui8Buffer[0];
            ui16ReadVerify <<= 8;
            ui16ReadVerify |= psInst->uCommand.pui8Buffer[1];
            if((ui16ReadVerify == 0) || (ui16ReadVerify == 0xFFFF))
            {
                psInst->pui8Data[0] = BMP180_O_AC1_MSB;
                I2CMRead(psInst->psI2CInst, psInst->ui8Addr, psInst->pui8Data,
                         1, psInst->uCommand.pui8Buffer, 22, BMP180Callback,
                         psInst);
            }
            else
            {
                psInst->i16AC1 =
                    (int16_t)((psInst->uCommand.pui8Buffer[0] << 8) |
                              psInst->uCommand.pui8Buffer[1]);
                psInst->i16AC2 =
                    (int16_t)((psInst->uCommand.pui8Buffer[2] << 8) |
                              psInst->uCommand.pui8Buffer[3]);
                psInst->i16AC3 =
                    (int16_t)((psInst->uCommand.pui8Buffer[4] << 8) |
                              psInst->uCommand.pui8Buffer[5]);
                psInst->ui16AC4 =
                    (uint16_t)((psInst->uCommand.pui8Buffer[6] << 8) |
                               psInst->uCommand.pui8Buffer[7]);
                psInst->ui16AC5 =
                    (uint16_t)((psInst->uCommand.pui8Buffer[8] << 8) |
                               psInst->uCommand.pui8Buffer[9]);
                psInst->ui16AC6 =
                    (uint16_t)((psInst->uCommand.pui8Buffer[10] << 8) |
                               psInst->uCommand.pui8Buffer[11]);
                psInst->i16B1 =
                    (int16_t)((psInst->uCommand.pui8Buffer[12] << 8) |
                              psInst->uCommand.pui8Buffer[13]);
                psInst->i16B2 =
                    (int16_t)((psInst->uCommand.pui8Buffer[14] << 8) |
                              psInst->uCommand.pui8Buffer[15]);
                psInst->i16MC =
                    (int16_t)((psInst->uCommand.pui8Buffer[18] << 8) |
                              psInst->uCommand.pui8Buffer[19]);
                psInst->i16MD =
                    (int16_t)((psInst->uCommand.pui8Buffer[20] << 8) |
                              psInst->uCommand.pui8Buffer[21]);
                psInst->ui8State = BMP180_STATE_IDLE;
            }

            break;
        }
        case BMP180_STATE_WRITE:
        {
            psInst->ui8Mode = psInst->ui8NewMode;
            psInst->ui8State = BMP180_STATE_IDLE;
            break;
        }
        case BMP180_STATE_RMW:
        {
            if(psInst->uCommand.sReadModifyWriteState.pui8Buffer[0] ==
               BMP180_O_CTRL_MEAS)
            {
                psInst->ui8Mode =
                    (psInst->uCommand.sReadModifyWriteState.pui8Buffer[1] &
                     BMP180_CTRL_MEAS_OSS_M);
            }
            psInst->ui8State = BMP180_STATE_IDLE;
            break;
        }

        case BMP180_STATE_REQ_TEMP:
        {
            I2CMRead(psInst->psI2CInst, psInst->ui8Addr,
                     psInst->uCommand.pui8Buffer, 1,
                     psInst->uCommand.pui8Buffer + 1, 1, BMP180Callback,
                     psInst);

            psInst->ui8State = BMP180_STATE_WAIT_TEMP;
            break;
        }
        case BMP180_STATE_WAIT_TEMP:
        {
            if(psInst->uCommand.pui8Buffer[1] & BMP180_CTRL_MEAS_SCO)
            {
                I2CMRead(psInst->psI2CInst, psInst->ui8Addr,
                         psInst->uCommand.pui8Buffer, 1,
                         psInst->uCommand.pui8Buffer + 1, 1, BMP180Callback,
                         psInst);
            }
            else
            {
                psInst->uCommand.pui8Buffer[0] = BMP180_O_OUT_MSB;
                I2CMRead(psInst->psI2CInst, psInst->ui8Addr,
                         psInst->uCommand.pui8Buffer, 1, psInst->pui8Data, 2,
                         BMP180Callback, psInst);
                psInst->ui8State = BMP180_STATE_READ_TEMP;
            }
            break;
        }

        case BMP180_STATE_READ_TEMP:
        {
            psInst->uCommand.pui8Buffer[0] = BMP180_O_CTRL_MEAS;
            psInst->uCommand.pui8Buffer[1] = (BMP180_CTRL_MEAS_SCO |
                                              BMP180_CTRL_MEAS_PRESSURE |
                                              psInst->ui8Mode);
            I2CMWrite(psInst->psI2CInst, psInst->ui8Addr,
                      psInst->uCommand.pui8Buffer, 2, BMP180Callback, psInst);
            psInst->ui8State = BMP180_STATE_REQ_PRES;
            break;
        }
        case BMP180_STATE_REQ_PRES:
        {
            I2CMRead(psInst->psI2CInst, psInst->ui8Addr,
                     psInst->uCommand.pui8Buffer, 1,
                     psInst->uCommand.pui8Buffer + 1, 1, BMP180Callback,
                     psInst);
            psInst->ui8State = BMP180_STATE_WAIT_PRES;
            break;
        }
        case BMP180_STATE_WAIT_PRES:
        {
            if(psInst->uCommand.pui8Buffer[1] & BMP180_CTRL_MEAS_SCO)
            {
                I2CMRead(psInst->psI2CInst, psInst->ui8Addr,
                         psInst->uCommand.pui8Buffer, 1,
                         psInst->uCommand.pui8Buffer + 1, 1, BMP180Callback,
                         psInst);
            }
            else
            {
                psInst->uCommand.pui8Buffer[0] = BMP180_O_OUT_MSB;
                I2CMRead(psInst->psI2CInst, psInst->ui8Addr,
                         psInst->uCommand.pui8Buffer, 1, psInst->pui8Data + 2,
                         3, BMP180Callback, psInst);
                psInst->ui8State = BMP180_STATE_READ_PRES;
            }
            break;
        }
    }
    if((psInst->ui8State == BMP180_STATE_IDLE) && psInst->pfnCallback)
    {
        psInst->pfnCallback(psInst->pvCallbackData, ui8Status);
    }
}

uint_fast8_t
BMP180Init(tBMP180 *psInst, tI2CMInstance *psI2CInst, uint_fast8_t ui8I2CAddr,
           tSensorCallback *pfnCallback, void *pvCallbackData)
{
    psInst->psI2CInst = psI2CInst;
    psInst->ui8Addr = ui8I2CAddr;
    psInst->ui8State = BMP180_STATE_INIT1;
    psInst->ui8Mode = 0;
    psInst->ui8NewMode = 0;
    psInst->pfnCallback = pfnCallback;
    psInst->pvCallbackData = pvCallbackData;
    psInst->pui8Data[0] = BMP180_O_SOFT_RESET;
    psInst->pui8Data[1] = BMP180_SOFT_RESET_VALUE;
    if(I2CMWrite(psI2CInst, ui8I2CAddr, psInst->pui8Data, 2, BMP180Callback,
                 psInst) == 0)
    {
        psInst->ui8State = BMP180_STATE_IDLE;
        return(0);
    }

    return(1);
}

uint_fast8_t
BMP180Read(tBMP180 *psInst, uint_fast8_t ui8Reg, uint8_t *pui8Data,
           uint_fast16_t ui16Count, tSensorCallback *pfnCallback,
           void *pvCallbackData)
{
    if(psInst->ui8State != BMP180_STATE_IDLE)
    {
        return(0);
    }

    psInst->pfnCallback = pfnCallback;
    psInst->pvCallbackData = pvCallbackData;
    psInst->ui8State = BMP180_STATE_READ;
    psInst->uCommand.pui8Buffer[0] = ui8Reg;
    if(I2CMRead(psInst->psI2CInst, psInst->ui8Addr,
                psInst->uCommand.pui8Buffer, 1, pui8Data, ui16Count,
                BMP180Callback, psInst) == 0)
    {
        psInst->ui8State = BMP180_STATE_IDLE;
        return(0);
    }

    return(1);
}

uint_fast8_t
BMP180Write(tBMP180 *psInst, uint_fast8_t ui8Reg, uint8_t *pui8Data,
            uint_fast16_t ui16Count, tSensorCallback *pfnCallback,
            void *pvCallbackData)
{
    if(psInst->ui8State != BMP180_STATE_IDLE)
    {
        return(0);
    }
    psInst->pfnCallback = pfnCallback;
    psInst->pvCallbackData = pvCallbackData;
    if((ui8Reg <= BMP180_O_CTRL_MEAS) &&
       ((ui8Reg + ui16Count) > BMP180_O_CTRL_MEAS))
    {
        psInst->ui8NewMode = (pui8Data[ui8Reg - BMP180_O_CTRL_MEAS] &
                              BMP180_CTRL_MEAS_OSS_M);
    }
    psInst->ui8State = BMP180_STATE_WRITE;

    if(I2CMWrite8(&(psInst->uCommand.sWriteState), psInst->psI2CInst,
                  psInst->ui8Addr, ui8Reg, pui8Data, ui16Count, BMP180Callback,
                  psInst) == 0)
    {
        psInst->ui8State = BMP180_STATE_IDLE;
        return(0);
    }
    return(1);
}

uint_fast8_t
BMP180ReadModifyWrite(tBMP180 *psInst, uint_fast8_t ui8Reg,
                      uint_fast8_t ui8Mask, uint_fast8_t ui8Value,
                      tSensorCallback *pfnCallback, void *pvCallbackData)
{
  
    if(psInst->ui8State != BMP180_STATE_IDLE)
    {
        return(0);
    }

    psInst->pfnCallback = pfnCallback;
    psInst->pvCallbackData = pvCallbackData;

    psInst->ui8State = BMP180_STATE_RMW;

    if(I2CMReadModifyWrite8(&(psInst->uCommand.sReadModifyWriteState),
                            psInst->psI2CInst, psInst->ui8Addr, ui8Reg,
                            ui8Mask, ui8Value, BMP180Callback, psInst) == 0)
    {
        psInst->ui8State = BMP180_STATE_IDLE;
        return(0);
    }

    return(1);
}

uint_fast8_t
BMP180DataRead(tBMP180 *psInst, tSensorCallback *pfnCallback,
               void *pvCallbackData)
{
    if(psInst->ui8State != BMP180_STATE_IDLE)
    {
        return(0);
    }
    psInst->pfnCallback = pfnCallback;
    psInst->pvCallbackData = pvCallbackData;
    psInst->ui8State = BMP180_STATE_REQ_TEMP;
    psInst->uCommand.pui8Buffer[0] = BMP180_O_CTRL_MEAS;
    psInst->uCommand.pui8Buffer[1] = (BMP180_CTRL_MEAS_SCO |
                                      BMP180_CTRL_MEAS_TEMPERATURE);
    if(I2CMWrite(psInst->psI2CInst, psInst->ui8Addr,
                 psInst->uCommand.pui8Buffer, 2, BMP180Callback, psInst) == 0)
    {
        psInst->ui8State = BMP180_STATE_IDLE;
        return(0);
    }
    return(1);
}

void
BMP180DataPressureGetRaw(tBMP180 *psInst, uint_fast32_t *pui32Pressure)
{

    *pui32Pressure = ((psInst->pui8Data[2] << 16) |
                      (psInst->pui8Data[3] << 8) |
                      (psInst->pui8Data[4] & BMP180_OUT_XLSB_M));
}

void
BMP180DataPressureGetFloat(tBMP180 *psInst, float *pfPressure)
{
    float fUT, fUP, fX1, fX2, fX3, fB3, fB4, fB5, fB6, fB7, fP;
    int_fast8_t i8Oss;

    i8Oss = psInst->ui8Mode >> BMP180_CTRL_MEAS_OSS_S;
    fUT = (float)(uint16_t)((psInst->pui8Data[0] << 8) |
                           psInst->pui8Data[1]);
    fUP = ((float)(int32_t)((psInst->pui8Data[2] << 16) |
                            (psInst->pui8Data[3] << 8) |
                            (psInst->pui8Data[4] & BMP180_OUT_XLSB_M)) /
           (1 << (8 - i8Oss)));
    fX1 = ((fUT - (float)psInst->ui16AC6) * (float)psInst->ui16AC5) / 32768.f;
    fX2 = ((float)psInst->i16MC * 2048.f) / (fX1 + (float)psInst->i16MD);
    fB5 = fX1 + fX2;
    fB6 = fB5 - 4000;
    fX1 = ((float)psInst->i16B2 * ((fB6 * fB6) / 4096)) / 2048;
    fX2 = ((float)psInst->i16AC2 * fB6) / 2048;
    fX3 = fX1 + fX2;
    fB3 = ((((float)psInst->i16AC1 * 4) + fX3) * (1 << i8Oss)) / 4;
    fX1 = ((float)psInst->i16AC3 * fB6) / 8192;
    fX2 = ((float)psInst->i16B1 * ((fB6 * fB6) / 4096)) / 65536;
    fX3 = (fX1 + fX2) / 4;
    fB4 = (float)psInst->ui16AC4 * ((fX3 / 32768) + 1);
    fB7 = (fUP - fB3) * (50000 >> i8Oss);
    fP = (fB7 * 2) / fB4;
    fX1 = (fP / 256) * (fP / 256);
    fX1 = (fX1 * 3038) / 65536;
    fX2 = (fP * -7357) / 65536;
    fP += (fX1 + fX2 + 3791) / 16;
    *pfPressure = fP;
}
void
BMP180DataTemperatureGetRaw(tBMP180 *psInst, uint_fast16_t *pui16Temperature)
{

    *pui16Temperature = (psInst->pui8Data[0] << 8) | psInst->pui8Data[1];
}
void
BMP180DataTemperatureGetFloat(tBMP180 *psInst, float *pfTemperature)
{
    float fUT, fX1, fX2, fB5;

    fUT = (float)(uint16_t)((psInst->pui8Data[0] << 8) |
                            psInst->pui8Data[1]);

    fX1 = ((fUT - (float)psInst->ui16AC6) * (float)psInst->ui16AC5) / 32768.f;
    fX2 = ((float)psInst->i16MC * 2048.f) / (fX1 + (float)psInst->i16MD);
    fB5 = fX1 + fX2;
    *pfTemperature = fB5 / 160.f;
}
