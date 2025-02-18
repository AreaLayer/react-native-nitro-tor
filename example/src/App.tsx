import { useEffect } from 'react';
import { Text, View, StyleSheet } from 'react-native';
import { RnMath } from 'react-native-nitro-tor';

const result = RnMath.add(3, 7);

export default function App() {
  // useEffect(() => {
  //   async function initTor() {
  //     try {
  //       console.log('Initializing Tor...');
  //       const servicePtr = await RnTor.createTorService(
  //         '/data/user/0/nitrotor.example/files', // dataDir
  //         19051, // socksPort
  //         45000 // bootstrapTimeoutMs
  //       );
  //       console.log('Tor service created, pointer:', servicePtr);
  //     } catch (error) {
  //       console.error('Error initializing Tor:', error);
  //     }
  //   }

  //   initTor();
  // }, []);

  return (
    <View style={styles.container}>
      <Text>Result: {result}</Text>
    </View>
  );
}

const styles = StyleSheet.create({
  container: {
    flex: 1,
    alignItems: 'center',
    justifyContent: 'center',
  },
});
