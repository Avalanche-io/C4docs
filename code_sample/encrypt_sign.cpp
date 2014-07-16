#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <gpgme.h>
#include <gpg-error.h>
#include <string.h>
#include <locale.h>
#include <errno.h>
#include<unistd.h>
#include<fstream>
#define SIZE 1024
#define MAXLEN 4096

 using namespace std;

 gpgme_ctx_t ctx;
 gpgme_data_t data,plain,cipher;

 int init_gpgme (void){

   char *p;
   gpgme_error_t err;
   gpgme_engine_info_t enginfo;

   /* The function `gpgme_check_version' must be called before any other
      function in the library, because it initializes the thread support
      subsystem in GPGME.*/

   p = (char *) gpgme_check_version(NULL);
   std::cout<<"version="<<p<<"\n";
    setlocale (LC_ALL, "");
   /* set locale */
   gpgme_set_locale(NULL, LC_CTYPE, setlocale(LC_CTYPE, NULL));

   /* check for OpenPGP support */
   err = gpgme_engine_check_version(GPGME_PROTOCOL_OpenPGP);
   if(err != GPG_ERR_NO_ERROR) return 1;

   p = (char *)gpgme_get_protocol_name(GPGME_PROTOCOL_OpenPGP);
   std::cout<<"Protocol name:"<<p<<"\n";

   /* get engine information */
   err = gpgme_get_engine_info(&enginfo);
   if(err != GPG_ERR_NO_ERROR) return 2;
   std::cout<<"file location= "<<enginfo->file_name<<"  version= "<<enginfo->version<< "\n";

    /* create context */
   err = gpgme_new(&ctx);
   if(err != GPG_ERR_NO_ERROR) return 3;

   /* do ascii armor data, so output is readable in console */
   gpgme_set_armor(ctx,1);

   /* set protocol to use in our context */
    err = gpgme_set_protocol(ctx,GPGME_PROTOCOL_OpenPGP);
    if(err != GPG_ERR_NO_ERROR)
    return 4;
    while (enginfo && enginfo->protocol != gpgme_get_protocol(ctx)) {
    enginfo = enginfo->next;
    }

   err = gpgme_ctx_set_engine_info (ctx, GPGME_PROTOCOL_OpenPGP,enginfo->file_name,NULL);
   if(err != GPG_ERR_NO_ERROR) return 5;

   return 0;

     }

  /*Generating keys*/
  void generate_keys(void){
   gpgme_genkey_result_t result;
   gpgme_key_t key;

   /*String containing parameters for signing in XML format*/
   const char *parms = "<GnupgKeyParms format=\"internal\"> \n Key-Type: RSA \n Key-Length: 1024 \n Subkey-Type: RSA \n Subkey-Length: 1024 \n Name-Real: Ojas Pandey \n Name-Comment: Original user \n Name-Email: abc@xyz.com \n Expire-Date: 0 \n Passphrase: abc \n </GnupgKeyParms>";
   std::cout<<"Parameters for generating keys"<<"\n"<< parms<<"\n";
   /* GPGMe function for generating keys */
   gpgme_error_t err= gpgme_op_genkey(ctx,parms,NULL,NULL);
   if(err== GPG_ERR_INV_VALUE)
   std::cout<<"parms is not a valid XML string";
   else if(err==GPG_ERR_NOT_SUPPORTED)
   std::cout<<"public and/or secret key invalid";
   else if(err== GPG_ERR_GENERAL)
   std::cout<<"Keys could not be created";
   /* initiating the signing process */
   err = gpgme_op_genkey_start(ctx,parms,NULL,NULL);
   if(err== GPG_ERR_INV_VALUE)
   std::cout<<"parms is not a valid XML string";
   else if(err==GPG_ERR_NOT_SUPPORTED)
   std::cout<<"public and/or secret key invalid";
   /* Wait for one thread to complete operation */
   ctx=gpgme_wait(ctx,&err,1);
   if(ctx==NULL)
    std::cout<<err;
    /* Store the results */
   result=gpgme_op_genkey_result(ctx);
   std::cout<< "Primary Key Fingerprint:"<<result->fpr<<"\n";
   std::cout<<"Generated keylist is:";
   err = gpgme_op_keylist_start (ctx,NULL,0);
     while (!err)
           {
             err = gpgme_op_keylist_next (ctx,&key);
             if (err)
               break;
             std::cout<<  key->subkeys->keyid<<"\t";
             if (key->uids && key->uids->name)
             std::cout<< key->uids->name<<"\t";
             if (key->uids && key->uids->email)
            std::cout<<key->uids->email<<"\n";
            }

}

  void export_keys(void){
   gpgme_data_t data;
   gpgme_error_t err;
   int tmp;
   char buf[SIZE];
   size_t read_bytes;
   char *p;
   /* create buffer for data exchange with gpgme*/
   err = gpgme_data_new(&data);
   if(err == GPG_ERR_INV_VALUE)
   std::cout<<"Invalid pointer for buffer";
   else if(err==GPG_ERR_ENOMEM)
   std::cout<<"Not enough memory available";

   /* set encoding for the buffer */
   err = gpgme_data_set_encoding(data,GPGME_DATA_ENCODING_ARMOR);

   if(err == GPGME_DATA_ENCODING_NONE)
   std::cout<<"Invalid pointer for encoding";

   /* verify encoding */
   tmp = gpgme_data_get_encoding(data);
   if(tmp == GPGME_DATA_ENCODING_ARMOR)
   std::cout<<"encode ok\n";
   else
   std::cout<<"encode broken\n";

   /* with NULL it exports all public keys */
    err = gpgme_op_export(ctx,NULL,0,data);
   if(err = GPG_ERR_INV_VALUE)
   std::cout<<"Data is not a valid empty buffer";

   read_bytes = gpgme_data_seek (data, 0, SEEK_END);
   std::cout<<"end is=\n"<<read_bytes;
   if(read_bytes == -1) {
      p = (char *) gpgme_strerror(errno);
      std::cout<<"data seek error:\n"<<p;

   }
   read_bytes = gpgme_data_seek (data, 0, SEEK_SET);
   std::cout<<"start is\n"<<read_bytes;

   /* write keys to stderr */
  while ((read_bytes = gpgme_data_read (data, buf, SIZE)) > 0) {
     write(2,buf,read_bytes);
   }
   /* append \n, so that there is really a line feed */
   write(2,"\n",1);

}

void delete_keys(void){
gpgme_error_t err;
gpgme_key_t key;

err = gpgme_op_keylist_start (ctx,NULL,0);

while (!err)
           {
             err = gpgme_op_keylist_next (ctx, &key);
             if (err)
               break;

            err= gpgme_op_delete_start(ctx,key,1);
            err= gpgme_op_delete (ctx, key,1);
            ctx=gpgme_wait(ctx,&err,1);
           }
            std::cout<<"done";
}

/*Creating Signature*/
void create_signature(void){
gpgme_sig_mode_t mode= GPGME_SIG_MODE_NORMAL;
gpgme_error_t* status;
gpgme_error_t err;
gpgme_data_t plain;
gpgme_data_t sign=NULL;
gpgme_sign_result_t sign_result;
gpgme_new_signature_t check;

err= gpgme_data_new_from_file(&plain,"file.txt",1);
if(err==GPG_ERR_NO_ERROR)
std::cout<<"Data object for plaintext created successfully"<<"\n";
else if(err==GPG_ERR_INV_VALUE)
std::cout<<"Invalid filename";
else if(err==GPG_ERR_ENOMEM)
std::cout<<"Not enough memory available";

/* initiating the signing process */
err=gpgme_op_sign_start(ctx, plain, sign, mode);

/* GPGMe function for creating signatures */
err=gpgme_op_sign(ctx, plain, sign, mode);

/* Wait for one thread to complete operation */
ctx=gpgme_wait(ctx,status,1);
if(err== GPG_ERR_NO_ERROR)
std::cout<<"Signature created successfully";
else if(err== GPG_ERR_NO_DATA)
std::cout<<"Error: The signature could not be created";
else if(err== GPG_ERR_BAD_PASSPHRASE)
std::cout<<"Error: The passphrase could not be retrieved successfully";
else if(err== GPG_ERR_UNUSABLE_SECKEY)
std::cout<<"Invalid signers";
else if(err== GPG_ERR_INV_VALUE)
std::cout<<"Invalid pointers";
/* Store the results */
sign_result=gpgme_op_sign_result(ctx);
std::cout<<"Signatures created are:"<< sign_result->signatures;
gpgme_data_release(plain);
}

/* Encrypting and signing the data*/
void encrypt_sign(void){
gpgme_error_t err;
gpgme_encrypt_flags_t flags= GPGME_ENCRYPT_ALWAYS_TRUST;
gpgme_key_t key;
gpgme_encrypt_result_t result;
gpgme_key_t recp[]={};
char *buffer;
ssize_t nbytes;
gpgme_key_t receiver;

recp[0]->uids->uid="abcd";
recp[0]->uids->name="John Smith";
recp[0]->uids->email="johnsmith@abc.com";

err = gpgme_op_keylist_start (ctx,NULL,0);
err = gpgme_op_keylist_next(ctx,&key);
err = gpgme_signers_add(ctx,key);
std::cout<<"Encryption by :"<< key->uids->name<<" "<< key->subkeys->keyid<<"\n";
//err = gpgme_op_keylist_next(ctx,&recp[0]);
std::cout<<"Encrypting for :"<< recp[0]->uids->name<< " "<<recp[0]->uids->email<<"\n";

err= gpgme_data_new_from_file(&plain,"file.txt", 1);
if(err==GPG_ERR_NO_ERROR)
std::cout<<"Data object for plaintext created successfully"<<"\n";
else if(err==GPG_ERR_INV_VALUE)
std::cout<<"Invalid filename";
else if(err==GPG_ERR_ENOMEM)
std::cout<<"Not enough memory available";
err = gpgme_data_new(&cipher);
if(err==GPG_ERR_NO_ERROR)
std::cout<<"Data object for ciphertext created successfully"<<"\n";
else if(err==GPG_ERR_INV_VALUE)
std::cout<<"Invalid filename";
else if(err==GPG_ERR_ENOMEM)
std::cout<<"Not enough memory available";

 /* set encoding for the buffer */
err = gpgme_data_set_encoding(cipher,GPGME_DATA_ENCODING_ARMOR);

/* Encrypting the data */
err=gpgme_op_encrypt_sign(ctx,recp,flags,plain,cipher);

/* Initiating the encryption and signing process */
err=gpgme_op_encrypt_sign_start(ctx,recp,flags,plain,cipher);
if(err==GPG_ERR_NO_ERROR )
std::cout<<"Operation started successfully"<<"\n";
else if(err==GPG_ERR_INV_VALUE)
std::cout<<"Invalid pointer";
else if(err==GPG_ERR_UNUSABLE_PUBKEY)
std::cout<<"Invalid recipients";

/* Wait for single thread to complete operation  */
ctx=gpgme_wait(ctx,&err,1);
if(err==GPG_ERR_NO_ERROR)
std::cout<<"Encryption successful"<<"\n";
else if(err==GPG_ERR_INV_VALUE)
std::cout<<"Invalid pointers";
else if(err==GPG_ERR_UNUSABLE_PUBKEY)
std::cout<<"Invalid recipients";
else if(err==GPG_ERR_BAD_PASSPHRASE )
std::cout<<"Passphrase for symmetric key could not be retrieved";
result=gpgme_op_encrypt_result(ctx);
if(result==NULL)
std::cout<<"Encryption Operation failed";
else if (result->invalid_recipients)
std::cout<< "Invalid recipient found:\n"<<result->invalid_recipients->fpr;
 nbytes = gpgme_data_seek (cipher, 0, SEEK_SET);
  if (nbytes == -1) {
    std::cout<<"Error in data seek: "<<	__FILE__<< __LINE__;
    perror("");
    exit (1);
    }
  buffer = (char*)malloc(MAXLEN);
  nbytes = gpgme_data_read(cipher, buffer, MAXLEN);
  if (nbytes == -1) {
    std::cout<< __FILE__<< __LINE__<< "Error in data read";
    exit (1);
  }
  buffer[nbytes] = '\0';
  std::cout<<"Encrypted text (bytes):"<< (int)nbytes<<"\n";
  std::cout<<buffer;
  /* Completed */

}


int main(int argc, char *argv[])
{
int return_init;
return_init=init_gpgme();
if(return_init==0)std::cout<<"Initialization successful"<<"\n";
else if(return_init==1)std::cout<<"Engine not available";
else if(return_init==2)std::cout<<"Memory for engine could not be allocated";
else if(return_init==3)std::cout<<"Context creation unsuccessful";
else if(return_init==4)std::cout<<"The protocol is invalid";
else if(return_init==5)std::cout<<"Engine setup failed";

 // generate_keys();

//export_keys();
//create_signature();
//delete_keys();
encrypt_sign();

   /* free data */
   gpgme_data_release(data);
   gpgme_data_release(plain);
   gpgme_data_release(cipher);

   /* free context */
   gpgme_release(ctx);


}








