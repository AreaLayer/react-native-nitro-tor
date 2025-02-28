import type { HybridObject } from 'react-native-nitro-modules';

// Define a fixed-size byte array type that Nitro can understand
type ByteArray64 = number[]; // This will be represented as std::array<uint8_t, 64> in C++

export interface TorConfig {
  socks_port: number;
  data_dir: string;
  timeout_ms: number;
}

export interface HiddenServiceParams {
  port: number;
  target_port: number;
  key_data?: ByteArray64; // Optional key data
}

export interface StartTorParams {
  data_dir: string;
  key_data?: ByteArray64;
  socks_port: number;
  target_port: number;
  timeout_ms: number;
}

export interface StartTorResponse {
  is_success: boolean;
  onion_address: string;
  control: string;
  error_message: string;
}

export interface HiddenServiceResponse {
  is_success: boolean;
  onion_address: string;
  control: string;
}

export interface HttpGetParams {
  url: string;
  headers: string;
  timeout_ms: number;
}

export interface HttpPostParams {
  url: string;
  body: string;
  headers: string;
  timeout_ms: number;
}

export interface HttpPutParams {
  url: string;
  body: string;
  headers: string;
  timeout_ms: number;
}

export interface HttpDeleteParams {
  url: string;
  headers: string;
  timeout_ms: number;
}

export interface HttpResponse {
  status_code: number;
  body: string;
  error: string;
}

export interface Tor extends HybridObject<{ ios: 'c++'; android: 'c++' }> {
  // Initialize the Tor service
  initTorService(config: TorConfig): Promise<boolean>;

  // Create a new hidden service
  createHiddenService(
    params: HiddenServiceParams
  ): Promise<HiddenServiceResponse>;

  // Start the Tor daemon with hidden service and control port
  startTorIfNotRunning(params: StartTorParams): Promise<StartTorResponse>;

  // Get the current service status
  getServiceStatus(): Promise<number>;

  // Delete an existing hidden service
  deleteHiddenService(onionAddress: string): Promise<boolean>;

  // Shutdown the Tor service
  shutdownService(): Promise<boolean>;

  // Http GET
  httpGet(params: HttpGetParams): Promise<HttpResponse>;

  // Http POST
  httpPost(params: HttpPostParams): Promise<HttpResponse>;

  // Http PUT
  httpPut(params: HttpPutParams): Promise<HttpResponse>;

  // Http Delete
  httpDelete(params: HttpDeleteParams): Promise<HttpResponse>;
}
