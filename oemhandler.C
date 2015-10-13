#include "oemhandler.h"
#include "ipmid-api.h"
#include <stdio.h>
#include <string.h>

void register_netfn_oem_partial_esel() __attribute__((constructor));



const char *g_esel_path = "/tmp/";
uint16_t g_record_id = 0x0100;


#define LSMSSWAP(x,y) (y<<8|x)


///////////////////////////////////////////////////////////////////////////////
// For the First partial add eSEL the SEL Record ID and offset 
// value should be 0x0000. The extended data needs to be in 
// the form of an IPMI SEL Event Record, with Event sensor type 
// of 0xDF and Event Message format of 0x04. The returned 
// Record ID should be used for all partial eSEL adds.
//
// This function creates a /tmp/esel# file to store the 
// incoming partial esel.  It is the role of some other
// function to commit the error log in to long term 
// storage.  Likely via the ipmi add_sel command.  
///////////////////////////////////////////////////////////////////////////////
ipmi_ret_t ipmi_ibm_oem_partial_esel(ipmi_netfn_t netfn, ipmi_cmd_t cmd, 
                              ipmi_request_t request, ipmi_response_t response, 
                              ipmi_data_len_t data_len, ipmi_context_t context)
{
    esel_request_t *reqptr = (esel_request_t*) request;
    FILE *fp;
    char string[32];
    short offset = 0, record=0;
    unsigned short rlen;
    ipmi_ret_t rc = IPMI_CC_OK;
    char iocmd[4];

    strcpy(string, g_esel_path);
    
    offset = LSMSSWAP(reqptr->offsetls, reqptr->offsetms);

    sprintf(string, "%s%s%02x%02x", string, "esel", reqptr->selrecordms, reqptr->selrecordls);


    // Length is the number of request bytes minus the header itself.
    // The header contains an extra byte to indicate the start of
    // the data (so didn't need to worry about word/byte boundaries)
    // hence the -1...
    rlen = ((unsigned short)*data_len) - (sizeof(esel_request_t));


    printf("IPMI PARTIAL ESEL for %s  Offset = %d Length = %d\n", 
        string, offset, rlen);

    if (offset == 0) {
        strcpy(iocmd, "wb");
    } else {
        // I was thinking "ab+" but it appears it doesn't
        // do what fseek asks.  Modify to rb+ and fseek 
        // works great...
        strcpy(iocmd, "rb+");
    }

    if ((fp = fopen(string, iocmd)) != NULL) {
        fseek(fp, offset, SEEK_SET);
        fwrite(reqptr+1,rlen,1,fp);
        fclose(fp);


        *data_len = sizeof(g_record_id);
        memcpy(response, &g_record_id, *data_len);


    } else {
        fprintf(stderr, "Error trying to perform %s for esel%s\n",iocmd, string);
        ipmi_ret_t rc = IPMI_CC_INVALID;   
        *data_len     = 0;
    }
    
    return rc;
}


void register_netfn_oem_partial_esel()
{
    printf("Registering NetFn:[0x%X], Cmd:[0x%X]\n",NETFUN_OEM_IBM, IPMI_CMD_PESEL);
    ipmi_register_callback(NETFUN_OEM_IBM, IPMI_CMD_PESEL, NULL, ipmi_ibm_oem_partial_esel);
    return;
}
