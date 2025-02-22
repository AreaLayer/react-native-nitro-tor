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

export interface Tor extends HybridObject<{ ios: 'c++'; android: 'c++' }> {
  // Initialize the Tor service
  initTorService(config: TorConfig): Promise<boolean>;

  // Create a new hidden service
  createHiddenService(params: HiddenServiceParams): Promise<string>;

  // Get the current service status
  getServiceStatus(): Promise<number>;

  // Delete an existing hidden service
  deleteHiddenService(onionAddress: string): Promise<boolean>;

  // Shutdown the Tor service
  shutdownService(): Promise<boolean>;
}
