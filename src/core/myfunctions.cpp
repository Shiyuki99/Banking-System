#include "myfunctions.hpp"

extern const size_t MAX_INPUT_SIZE = 64;

char *getInput() {

   char *buffer = new char[MAX_INPUT_SIZE - 1];
   std::cin.get(buffer, MAX_INPUT_SIZE);

   while (std::cin.gcount() > MAX_INPUT_SIZE) {

      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      std::cout << "Please do not enter more than 64 character :)" << std::endl;

      std::cin.get(buffer, MAX_INPUT_SIZE);
   }

   return buffer;

}

char *toHexString(const unsigned char *data, size_t length) {
   // Allocate a char array to store the hex string (2 characters per byte + null terminator)
   size_t hexLength = length * 2 + 1;
   char *hexString = new char[hexLength];
   char *ptr = hexString;

   for (size_t i = 0; i < length; ++i) {
      // Convert each byte to two hex characters and append them to the string
      sprintf(ptr, "%02X", data[i]);
      ptr += 2; // Move the pointer forward
   }

   hexString[hexLength - 1] = '\0'; // Null-terminate the string
   return hexString;
}

char *hashString(const char *string) {
   // Create an EVP context
   EVP_MD_CTX *ctx = EVP_MD_CTX_new();
   if (!ctx) {
      return nullptr; // Handle allocation failure
   }
   const EVP_MD *md = EVP_sha256(); // Specify the hashing algorithm
   unsigned char *hash = (unsigned char *)malloc(sizeof(unsigned char) * EVP_MAX_MD_SIZE);
   unsigned int hash_len = 0;

   // Initialize the context
   if (EVP_DigestInit_ex(ctx, md, nullptr) != 1) {
      EVP_MD_CTX_free(ctx);
      return nullptr; // Handle initialization failure
   }

   // Update the context with the input
   if (EVP_DigestUpdate(ctx, string, std::strlen(string)) != 1) {
      EVP_MD_CTX_free(ctx);
      return nullptr; // Handle update failure
   }

   // Finalize the digest
   if (EVP_DigestFinal_ex(ctx, hash, &hash_len) != 1) {
      EVP_MD_CTX_free(ctx);
      return nullptr; // Handle finalization failure
   }

   EVP_MD_CTX_free(ctx);


   return toHexString(hash, hash_len);
}
