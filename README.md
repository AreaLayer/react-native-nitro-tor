# react-native-nitro-tor

A Tor Daemon and Onion Routing Client for React Native using pure C++ [NitroModules](https://github.com/mrousavy/nitro).

## Features

- Run a Tor daemon directly in your React Native application
- Create and manage Tor hidden services
- Built with performance in mind using React Native's NitroModules
- Cross-platform support for iOS and macOS

## Installation

```bash
# Using npm
npm install react-native-nitro-tor

# Using yarn
yarn add react-native-nitro-tor
```

You'll also need to install the dependency:

```bash
npm install react-native-nitro-modules
# or
yarn add react-native-nitro-modules
```

## Platform Support

| Platform | Support                     |
| -------- | --------------------------- |
| iOS      | ✅                          |
| macOS    | ✅                          |
| Android  | ✅ (arm64-v8a, x86_64, x86) |

## Usage

### Basic Example

```typescript
import { TorModule } from 'react-native-nitro-tor';

// Start Tor with a hidden service
const startTor = async () => {
  try {
    const result = await TorModule.startTorIfNotRunning({
      data_dir: '/path/to/tor/data',
      socks_port: 9050,
      target_port: 8080,
      timeout_ms: 60000,
    });

    if (result.is_success) {
      console.log(`Tor started successfully!`);
      console.log(`Onion address: ${result.onion_address}`);
      console.log(`Control: ${result.control}`);
    } else {
      console.error(`Failed to start Tor: ${result.error_message}`);
    }
  } catch (error) {
    console.error('Error starting Tor:', error);
  }
};
```

### Advanced Usage

```typescript
import { TorModule } from 'react-native-nitro-tor';

// Initialize Tor service
const initTor = async () => {
  const initialized = await TorModule.initTorService({
    socks_port: 9050,
    data_dir: '/path/to/tor/data',
    timeout_ms: 60000,
  });

  if (initialized) {
    console.log('Tor service initialized successfully');
    return true;
  }
  return false;
};

// Create a hidden service
const createService = async () => {
  const serviceResult = await TorModule.createHiddenService({
    port: 80,
    target_port: 8080,
    // Optionally provide key_data for persistent services
  });

  if (serviceResult.is_success) {
    console.log(`Created hidden service at: ${serviceResult.onion_address}`);
  }
};

// Check service status
const checkStatus = async () => {
  const status = await TorModule.getServiceStatus();
  console.log(`Current Tor service status: ${status}`);
};

// Shutdown Tor service
const shutdown = async () => {
  const result = await TorModule.shutdownService();
  console.log(`Tor shutdown ${result ? 'successful' : 'failed'}`);
};
```

## API Reference

### Types

```typescript
type ByteArray64 = number[];

interface TorConfig {
  socks_port: number;
  data_dir: string;
  timeout_ms: number;
}

interface HiddenServiceParams {
  port: number;
  target_port: number;
  key_data?: ByteArray64;
}

interface StartTorParams {
  data_dir: string;
  key_data?: ByteArray64;
  socks_port: number;
  target_port: number;
  timeout_ms: number;
}

interface StartTorResponse {
  is_success: boolean;
  onion_address: string;
  control: string;
  error_message: string;
}

interface HiddenServiceResponse {
  is_success: boolean;
  onion_address: string;
  control: string;
}
```

### Methods

- `initTorService(config: TorConfig): Promise<boolean>`  
  Initialize the Tor service with the given configuration.

- `createHiddenService(params: HiddenServiceParams): Promise<HiddenServiceResponse>`  
  Create a new Tor hidden service with the specified parameters.

- `startTorIfNotRunning(params: StartTorParams): Promise<StartTorResponse>`  
  Start the Tor daemon with a hidden service if it's not already running. This is the recommended method for most use cases.

- `getServiceStatus(): Promise<number>`  
  Get the current status of the Tor service.

- `deleteHiddenService(onionAddress: string): Promise<boolean>`  
  Delete an existing hidden service by its onion address.

- `shutdownService(): Promise<boolean>`  
  Completely shut down the Tor service.

## Binary Files

- iOS and MacOS: Binaries are located in the root of the project as `Tor.xcframework`
- Android: Binaries are located in `android/src/main/jniLibs`

## Architecture Support

- Android: arm64-v8a, x86_64, x86

## License

MIT

## Credits

This project builds upon the work of:

- [react-native-tor](https://github.com/Sifir-io/react-native-tor)
- [sifir-rs-sdk](https://github.com/Sifir-io/sifir-rs-sdk/)
- [libtor](https://github.com/MagicalBitcoin/libtor)

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.
