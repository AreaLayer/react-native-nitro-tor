# react-native-nitro-tor

A Tor Daemon and Onion Routing Client for React Native using pure C++ [NitroModules](https://github.com/mrousavy/nitro).

## Features

- Run a Tor daemon directly in your React Native application
- Create and manage Tor hidden services
- Make HTTP requests over the Tor network (GET, POST, PUT, DELETE)
- Built with performance in mind using React Native's NitroModules
- Cross-platform support for Android, iOS and macOS

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
import { RnTor } from 'react-native-nitro-tor';

// Start Tor with a hidden service
const startTor = async () => {
  try {
    const result = await RnTor.startTorIfNotRunning({
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

// Shut down the Tor service
const shutdown = async () => {
  const result = await RnTor.shutdownService();
  console.log(`Tor shutdown ${result ? 'successful' : 'failed'}`);
};
```

### HTTP Methods Over Tor

```typescript
import { RnTor } from 'react-native-nitro-tor';

// Make an HTTP GET request through Tor
const makeGetRequest = async () => {
  try {
    const result = await RnTor.httpGet({
      url: 'http://example.com',
      headers: '',
      timeout_ms: 2000,
    });
    console.log(`Status code: ${result.status_code}`);
    console.log(`Response body: ${result.body}`);
    if (result.error) {
      console.error(`Error: ${result.error}`);
    }
  } catch (error) {
    console.error('Error making GET request:', error);
  }
};

// Make an HTTP POST request through Tor
const makePostRequest = async () => {
  try {
    const result = await RnTor.httpPost({
      url: 'http://httpbin.org/post',
      body: '{"test":"data"}',
      headers: '{"Content-Type":"application/json"}',
      timeout_ms: 2000,
    });
    console.log(`Status code: ${result.status_code}`);
    console.log(`Response body: ${result.body}`);
    if (result.error) {
      console.error(`Error: ${result.error}`);
    }
  } catch (error) {
    console.error('Error making POST request:', error);
  }
};

// Make an HTTP PUT request through Tor
const makePutRequest = async () => {
  try {
    const result = await RnTor.httpPut({
      url: 'http://httpbin.org/put',
      body: '{"updated":"value"}',
      headers: '{"Content-Type":"application/json"}',
      timeout_ms: 2000,
    });
    console.log(`Status code: ${result.status_code}`);
    console.log(`Response body: ${result.body}`);
    if (result.error) {
      console.error(`Error: ${result.error}`);
    }
  } catch (error) {
    console.error('Error making PUT request:', error);
  }
};

// Make an HTTP DELETE request through Tor
const makeDeleteRequest = async () => {
  try {
    const result = await RnTor.httpDelete({
      url: 'http://httpbin.org/delete',
      headers: '{"Content-Type":"application/json"}',
      timeout_ms: 2000,
    });
    console.log(`Status code: ${result.status_code}`);
    console.log(`Response body: ${result.body}`);
    if (result.error) {
      console.error(`Error: ${result.error}`);
    }
  } catch (error) {
    console.error('Error making DELETE request:', error);
  }
};
```

### Advanced Usage

```typescript
import { RnTor } from 'react-native-nitro-tor';
// Initialize Tor service
const initTor = async () => {
  const initialized = await RnTor.initTorService({
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
  const serviceResult = await RnTor.createHiddenService({
    port: 9055,
    target_port: 9056,
    // Optionally provide key_data for persistent services
  });

  if (serviceResult.is_success) {
    console.log(`Created hidden service at: ${serviceResult.onion_address}`);
  }
};

// Get the current status of the Tor service.
// 0: Tor is in the process of starting.
// 1: Tor is running.
// 2: Stopped/Not running/error.

// Check service status
const checkStatus = async () => {
  const status = await RnTor.getServiceStatus();
  console.log(`Current Tor service status: ${status}`);
};

// Shutdown Tor service
const shutdown = async () => {
  const result = await RnTor.shutdownService();
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

interface HttpGetParams {
  url: string;
  headers: string;
  timeout_ms: number;
}

interface HttpPostParams {
  url: string;
  body: string;
  headers: string;
  timeout_ms: number;
}

interface HttpPutParams {
  url: string;
  body: string;
  headers: string;
  timeout_ms: number;
}

interface HttpDeleteParams {
  url: string;
  headers: string;
  timeout_ms: number;
}

interface HttpResponse {
  status_code: number;
  body: string;
  error: string;
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
  `0`: Tor is in the process of starting.
  `1`: Tor is running.
  `2`: Stopped/Not running/error.

- `deleteHiddenService(onionAddress: string): Promise<boolean>`  
  Delete an existing hidden service by its onion address.

- `shutdownService(): Promise<boolean>`  
  Completely shut down the Tor service.

- `httpGet(params: HttpGetParams): Promise<HttpResponse>`  
  Make an HTTP GET request through the Tor network.

- `httpPost(params: HttpPostParams): Promise<HttpResponse>`  
  Make an HTTP POST request through the Tor network.

- `httpPut(params: HttpPutParams): Promise<HttpResponse>`  
  Make an HTTP PUT request through the Tor network.

- `httpDelete(params: HttpDeleteParams): Promise<HttpResponse>`  
  Make an HTTP DELETE request through the Tor network.

## Binary Files

- iOS and MacOS: Binaries are located in the root of the project as `Tor.xcframework`
- Android: Binaries are located in `android/src/main/jniLibs`

## Architecture Support

- Android: arm64-v8a, x86_64, x86

## Running the example app

```
# Install dependencies
yarn install

# Generate native interfaces
yarn nitrogen

# Start metro
yarn example start

# For android
yarn example android

# For ios
yarn example ios

# IF ABOVE STEP FOR IOS THROWS ERRORS, you can also try:
cd example/ios && pod install

- Open the NitroTorExample.xcworkspace inside of xcode.
- Drag the Tor.xcframework from the root of the project to xcode project and select the "Copy files to destination" option.
- Build and run from inside of xcode.
```

## License

MIT

## Credits

This project builds upon the work of:

- [react-native-tor](https://github.com/Sifir-io/react-native-tor)
- [sifir-rs-sdk](https://github.com/Sifir-io/sifir-rs-sdk/)
- [libtor](https://github.com/MagicalBitcoin/libtor)

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.
