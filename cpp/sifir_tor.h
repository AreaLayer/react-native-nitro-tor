#include <cstdarg>
#include <cstdint>
#include <cstdlib>
#include <new>
#include <ostream>

struct OwnedTorService;

struct TorHiddenService;

struct TorService;

extern "C" {

/// Creates a new TorService instance.
///
/// # Parameters
///
/// * `data_dir_ptr`: Pointer to a C-style string representing the data directory path. C++ is
/// responsible for ensuring this pointer is valid and null-terminated.
/// * `socks_port`: The SOCKS port for the Tor service.
/// * `bootstrap_timeout_ms`: Bootstrap timeout in milliseconds.
///
/// # Returns
///
/// * `*mut TorService`: A raw pointer to the created TorService instance on success. Returns `null`
/// on failure.
///
/// # Safety
///
/// * The `data_dir_ptr` must be a valid, null-terminated C-style string.
/// * C++ is responsible for freeing the memory allocated for `TorService` using `free_tor_service`.
TorService *create_tor_service(const char *data_dir_ptr, uint16_t socks_port,
                               uint64_t bootstrap_timeout_ms);

/// Converts a TorService to an OwnedTorService, taking ownership and bootstrapping the Tor daemon.
///
/// # Parameters
///
/// * `service_ptr`: Pointer to a TorService instance created by `create_tor_service`.
///
/// # Returns
///
/// * `*mut OwnedTorService`: A raw pointer to the bootstrapped OwnedTorService instance on success.
/// Returns `null` on failure.
///
/// # Safety
///
/// * `service_ptr` must be a valid pointer to a TorService instance created by
/// `create_tor_service`.
/// * After calling this function, the memory pointed to by `service_ptr` is considered consumed and
/// should not be accessed again.
/// * C++ is responsible for freeing the memory allocated for `OwnedTorService` using
/// `free_owned_tor_service`.
OwnedTorService *into_owned_tor_service(TorService *service_ptr);

/// Frees the memory allocated for a TorService instance.
///
/// # Parameters
///
/// * `service_ptr`: Pointer to a TorService instance. Can be `null`.
///
/// # Safety
///
/// * `service_ptr` must be a pointer returned by `create_tor_service`.
/// * It is safe to call this function with a null pointer (it will do nothing).
void free_tor_service(TorService *service_ptr);

/// Gets the bootstrap status of the OwnedTorService.
///
/// # Parameters
///
/// * `owned_service_ptr`: Pointer to an OwnedTorService instance.
///
/// # Returns
///
/// * `*mut c_char`: A C-style string describing the bootstrap status. C++ is responsible for
/// freeing this string using `free_cstring`.
///    Returns a string indicating an error if status retrieval fails.
///
/// # Safety
///
/// * `owned_service_ptr` must be a valid pointer to an OwnedTorService instance created by
/// `into_owned_tor_service`.
char *get_tor_status(OwnedTorService *owned_service_ptr);

/// Creates a hidden service using the OwnedTorService.
///
/// # Parameters
///
/// * `owned_service_ptr`: Pointer to an OwnedTorService instance.
/// * `to_port`: The local port to forward traffic to for the hidden service.
/// * `hs_port`: The hidden service port to expose.
/// * `secret_key_ptr`: Optional pointer to a 64-byte secret key. If `null`, a new key is generated.
///
/// # Returns
///
/// * `*mut TorHiddenService`: A raw pointer to the created TorHiddenService instance on success.
/// Returns `null` on failure.
///
/// # Safety
///
/// * `owned_service_ptr` must be a valid pointer to an OwnedTorService instance.
/// * `secret_key_ptr`, if not null, must point to a valid 64-byte array.
/// * C++ is responsible for freeing the memory allocated for `TorHiddenService` using
/// `free_tor_hidden_service`.
TorHiddenService *create_hidden_service(OwnedTorService *owned_service_ptr, uint16_t to_port,
                                        uint16_t hs_port, const uint8_t *secret_key_ptr);

/// Deletes a hidden service.
///
/// # Parameters
///
/// * `owned_service_ptr`: Pointer to an OwnedTorService instance.
/// * `onion_ptr`: Pointer to a C-style string representing the onion address to delete.
///
/// # Returns
///
/// * `*mut c_char`: A C-style string indicating the result of the operation.
///    Returns "Ok" on success, or an error message on failure. C++ is responsible for freeing this
///    string using `free_cstring`.
///
/// # Safety
///
/// * `owned_service_ptr` must be a valid pointer to an OwnedTorService instance.
/// * `onion_ptr` must be a valid, null-terminated C-style string.
char *delete_hidden_service(OwnedTorService *owned_service_ptr, const char *onion_ptr);

/// Frees the memory allocated for an OwnedTorService instance and shuts down the Tor daemon.
///
/// # Parameters
///
/// * `owned_service_ptr`: Pointer to an OwnedTorService instance. Can be `null`.
///
/// # Safety
///
/// * `owned_service_ptr` must be a pointer returned by `into_owned_tor_service`.
/// * It is safe to call this function with a null pointer (it will do nothing).
void free_owned_tor_service(OwnedTorService *owned_service_ptr);

/// Returns the Onion URL of a TorHiddenService as a C-style string.
///
/// # Parameters
///
/// * `hidden_service_ptr`: Pointer to a TorHiddenService instance.
///
/// # Returns
///
/// * `*mut c_char`: A C-style string containing the Onion URL. C++ is responsible for freeing this
/// string using `free_cstring`.
///    Returns a string indicating an error if URL retrieval fails.
///
/// # Safety
///
/// * `hidden_service_ptr` must be a valid pointer to a TorHiddenService instance created by
/// `create_hidden_service`.
char *get_hidden_service_onion_url(TorHiddenService *hidden_service_ptr);

/// Returns the Secret Key of a TorHiddenService as a C-style string.
///
/// # Parameters
///
/// * `hidden_service_ptr`: Pointer to a TorHiddenService instance.
///
/// # Returns
///
/// * `*mut c_char`: A C-style string containing the Secret Key (hex encoded for safety in string).
/// C++ is responsible for freeing this string using `free_cstring`.
///    Returns a string indicating an error if key retrieval fails.
///
/// # Safety
///
/// * `hidden_service_ptr` must be a valid pointer to a TorHiddenService instance created by
/// `create_hidden_service`.
char *get_hidden_service_secret_key(TorHiddenService *hidden_service_ptr);

/// Frees the memory allocated for a TorHiddenService instance.
///
/// # Parameters
///
/// * `hidden_service_ptr`: Pointer to a TorHiddenService instance. Can be `null`.
///
/// # Safety
///
/// * `hidden_service_ptr` must be a pointer returned by `create_hidden_service`.
/// * It is safe to call this function with a null pointer (it will do nothing).
void free_tor_hidden_service(TorHiddenService *hidden_service_ptr);

/// Frees memory allocated for a C-style string that was created in Rust and passed to C++.
///
/// # Parameters
///
/// * `cstr_ptr`: Pointer to a C-style string. Can be `null`.
///
/// # Safety
///
/// * `cstr_ptr` must be a pointer returned by a Rust FFI function that returns `*mut c_char`.
/// * It is safe to call this function with a null pointer (it will do nothing).
void free_cstring(char *cstr_ptr);

} // extern "C"
