import { useEffect, useState } from 'react';
import { Text, View, StyleSheet } from 'react-native';
import * as RNFS from '@dr.pogodin/react-native-fs';
import { RnTor } from 'react-native-nitro-tor';

// Constants
const TOR_DATA_PATH = `${RNFS.DocumentDirectoryPath}/tor_data`;

interface TorState {
  isInitialized: boolean | undefined;
  status: number | undefined;
  onionUrl: string | undefined;
  error?: string;
}

interface HiddenServiceConfig {
  port: number;
  target_port: number;
}

const TOR_CONFIG = {
  socks_port: 9050,
  data_dir: TOR_DATA_PATH,
  timeout_ms: 60000,
};

const HIDDEN_SERVICE_CONFIG: HiddenServiceConfig = {
  port: 9055,
  target_port: 9056,
};

export default function TorApp() {
  const [torState, setTorState] = useState<TorState>({
    isInitialized: undefined,
    status: undefined,
    onionUrl: undefined,
  });

  useEffect(() => {
    const initTor = async () => {
      try {
        console.log('Starting Tor initialization with path:', TOR_DATA_PATH);

        // Ensure directory exists
        await ensureDataDirectory();

        // Initialize service
        const isInitialized = await initializeTorService();
        if (!isInitialized) {
          throw new Error('Failed to initialize Tor service');
        }

        // Check status
        const status = await checkTorStatus();

        // Setup hidden service
        const onionUrl = await setupHiddenService();

        setTorState({
          isInitialized: true,
          status,
          onionUrl,
        });
      } catch (error) {
        console.error('Error in Tor initialization:', error);
        setTorState((prev) => ({
          ...prev,
          error: error.message,
          isInitialized: false,
        }));
      }
    };

    initTor();

    // Cleanup on unmount
    return () => {
      RnTor.shutdownService().catch(console.error);
    };
  }, []);

  const ensureDataDirectory = async () => {
    try {
      console.log('Checking directory:', TOR_DATA_PATH);

      const exists = await RNFS.exists(TOR_DATA_PATH);
      if (!exists) {
        console.log('Creating Tor data directory...');
        await RNFS.mkdir(TOR_DATA_PATH, {
          NSURLIsExcludedFromBackupKey: true, // iOS specific
        });
      }

      // Verify directory is writable by trying to create and remove a test file
      const testFile = `${TOR_DATA_PATH}/.test`;
      try {
        await RNFS.writeFile(testFile, 'test', 'utf8');
        await RNFS.unlink(testFile);
        console.log('Directory is writable');
      } catch (writeError) {
        throw new Error(`Directory is not writable: ${writeError.message}`);
      }

      console.log('Directory ready:', TOR_DATA_PATH);
    } catch (error) {
      console.error('Error with directory setup:', error);
      throw new Error(`Failed to setup data directory: ${error.message}`);
    }
  };

  const initializeTorService = async () => {
    console.log('Initializing Tor with config:', TOR_CONFIG);
    try {
      const result = await RnTor.initTorService(TOR_CONFIG);
      console.log('Tor initialization result:', result);
      return result;
    } catch (error) {
      console.error('Tor initialization error:', error);
      throw new Error(`Tor initialization failed: ${error.message}`);
    }
  };

  const checkTorStatus = async () => {
    try {
      const status = await RnTor.getServiceStatus();
      console.log('Tor status:', status);
      return status;
    } catch (error) {
      console.error('Failed to get Tor status:', error);
      throw new Error(`Failed to get Tor status: ${error.message}`);
    }
  };

  const setupHiddenService = async () => {
    try {
      const onionUrl = await RnTor.createHiddenService(HIDDEN_SERVICE_CONFIG);
      console.log('Hidden service created:', onionUrl);
      return onionUrl;
    } catch (error) {
      console.error('Failed to create hidden service:', error);
      throw new Error(`Failed to create hidden service: ${error.message}`);
    }
  };

  return (
    <View style={styles.container}>
      <Text style={styles.statusText}>
        Init Tor Result: {String(torState.isInitialized)}
      </Text>
      <Text style={styles.statusText}>Tor Status: {torState.status}</Text>
      <Text style={styles.statusText}>
        Onion URL: {torState.onionUrl || 'Not available'}
      </Text>
      {torState.error && (
        <Text style={[styles.statusText, styles.errorText]}>
          Error: {torState.error}
        </Text>
      )}
    </View>
  );
}

const styles = StyleSheet.create({
  container: {
    flex: 1,
    alignItems: 'center',
    justifyContent: 'center',
    padding: 20,
  },
  statusText: {
    fontSize: 16,
    marginVertical: 8,
    textAlign: 'center',
  },
});
