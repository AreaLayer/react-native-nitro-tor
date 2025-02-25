import { useEffect, useState } from 'react';
import { Text, View, StyleSheet, Button, Platform } from 'react-native';
import * as RNFS from '@dr.pogodin/react-native-fs';
import { RnTor } from 'react-native-nitro-tor';

import { toJson } from '@bufbuild/protobuf';

import { start, getInfo } from 'react-native-turbo-lnd';
import { GetInfoResponseSchema } from 'react-native-turbo-lnd/protos/lightning_pb';

// Constants
const TOR_DATA_PATH = `${RNFS.DocumentDirectoryPath}/tor_data`;
const LND_DATA_PATH = `${RNFS.DocumentDirectoryPath}/lnd_data`;

interface TorState {
  isSuccess: boolean | undefined;
  errorMessage: string | undefined;
  onionUrl: string | undefined;
  controlUrl: string | undefined;
}

export default function TorApp() {
  const [torState, setTorState] = useState<TorState>({
    isSuccess: undefined,
    errorMessage: undefined,
    onionUrl: undefined,
    controlUrl: undefined,
  });

  useEffect(() => {
    const initTor = async () => {
      try {
        console.log('Starting Tor initialization with path:', TOR_DATA_PATH);

        // Ensure directory exists
        await ensureDataDirectory();

        // Initialize service
        const result = await RnTor.startTorIfNotRunning({
          data_dir: TOR_DATA_PATH,
          socks_port: 9050,
          target_port: 9051,
          timeout_ms: 60000,
        });
        if (!result.is_success) {
          throw new Error('Failed to initialize Tor service');
        }

        console.log(result);

        setTorState({
          isSuccess: result.is_success,
          errorMessage: result.error_message,
          onionUrl: result.onion_address,
          controlUrl: result.control,
        });
      } catch (error: any) {
        console.error('Error in Tor initialization:', error);
        setTorState((prev) => ({
          ...prev,
          errorMessage: error.message,
          isSuccess: false,
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
      } catch (writeError: any) {
        throw new Error(`Directory is not writable: ${writeError.message}`);
      }

      console.log('Directory ready:', TOR_DATA_PATH);
    } catch (error: any) {
      console.error('Error with directory setup:', error);
      throw new Error(`Failed to setup data directory: ${error.message}`);
    }
  };

  return (
    <View style={styles.container}>
      <Text style={styles.statusText}>
        Init Tor Result: {String(torState.isSuccess)}
      </Text>
      <Text style={styles.statusText}>Tor Status: {torState.isSuccess}</Text>
      <Text style={styles.statusText}>
        Onion URL: {torState.onionUrl || 'Not available'}
      </Text>
      <Text style={styles.statusText}>
        Onion URL: {torState.controlUrl || 'Not available'}
      </Text>
      {!torState.isSuccess && (
        <Text style={styles.statusText}>Error: {torState.errorMessage}</Text>
      )}

      <Button
        title="start"
        onPress={async () => {
          let lnddir: string = '';
          if (Platform.OS === 'android') {
            lnddir = LND_DATA_PATH;
          } else if (Platform.OS === 'ios') {
            lnddir = RNFS.LibraryDirectoryPath + '/Application Support/lnd/';
          } else if (Platform.OS === 'macos') {
            lnddir = RNFS.LibraryDirectoryPath + '/Application Support/lnd/';
          }

          try {
            await start(
              `--lnddir="${lnddir}"
                --noseedbackup
                --listen=localhost
                --bitcoin.active
                --bitcoin.mainnet
                --bitcoin.node=neutrino
                --feeurl="https://nodes.lightning.computer/fees/v1/btc-fee-estimates.json"
                --routing.assumechanvalid
                --tlsdisableautofill
                --db.bolt.auto-compact
                --db.bolt.auto-compact-min-age=0
                --neutrino.connect=neutrino.noderunner.wtf:8333
                --tor.active
                --tor.v3
                --tor.socks=127.0.0.1:9050
                --tor.control=${torState.controlUrl}
                `
            );
          } catch (err) {
            console.error(err);
          }
        }}
      />

      <Button
        title="getInfo"
        onPress={async () => {
          const result = await getInfo({});
          console.log('getInfo', toJson(GetInfoResponseSchema, result));
        }}
      />
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
