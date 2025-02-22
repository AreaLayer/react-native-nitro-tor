import { useEffect, useState } from 'react';
import { Text, View, StyleSheet } from 'react-native';
import { RnTor } from 'react-native-nitro-tor';
import RNFS from 'react-native-fs';
import type { TorConfig } from '../../src/Tor.nitro';

// Types
interface TorState {
  isInitialized: boolean | undefined;
  status: number | undefined;
  onionUrl: string | undefined;
}

interface HiddenServiceConfig {
  port: number;
  target_port: number;
}

// Constants
const TOR_DATA_PATH = `${RNFS.DocumentDirectoryPath}/tor_data`;
const TOR_CONFIG: TorConfig = {
  socks_port: 9050,
  data_dir: TOR_DATA_PATH,
  timeout_ms: 60000,
};
const HIDDEN_SERVICE_CONFIG: HiddenServiceConfig = {
  port: 9055,
  target_port: 9056,
};

export default function TorApp() {
  // State
  const [torState, setTorState] = useState<TorState>({
    isInitialized: undefined,
    status: undefined,
    onionUrl: undefined,
  });

  // Initialize Tor service
  useEffect(() => {
    const initTor = async () => {
      try {
        await ensureDataDirectory();
        await initializeTorService();
        await setupHiddenService();
      } catch (error) {
        console.error('Error in Tor initialization:', error);
        // You might want to add error state handling here
      }
    };

    initTor();
  }, []);

  // Helper functions
  const ensureDataDirectory = async () => {
    const exists = await RNFS.exists(TOR_DATA_PATH);
    if (!exists) {
      await RNFS.mkdir(TOR_DATA_PATH);
    }
  };

  const initializeTorService = async () => {
    console.log('Initializing Tor...');
    const isInitialized = await RnTor.initTorService(TOR_CONFIG);
    const status = await RnTor.getServiceStatus();

    setTorState((prev) => ({
      ...prev,
      isInitialized,
      status,
    }));

    console.log('Tor initialization complete:', { isInitialized, status });
  };

  const setupHiddenService = async () => {
    const onionUrl = await RnTor.createHiddenService(HIDDEN_SERVICE_CONFIG);

    setTorState((prev) => ({
      ...prev,
      onionUrl,
    }));

    console.log('Hidden service created:', onionUrl);
  };

  // Render
  return (
    <View style={styles.container}>
      <Text style={styles.statusText}>
        Init Tor Result: {String(torState.isInitialized)}
      </Text>
      <Text style={styles.statusText}>Tor Status: {torState.status}</Text>
      <Text style={styles.statusText}>
        Onion URL: {torState.onionUrl || 'Not available'}
      </Text>
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
