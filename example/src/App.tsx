import { Text, View, StyleSheet } from 'react-native';
import { RnMath } from 'react-native-nitro-tor';

const result = RnMath.add(3, 7);

export default function App() {
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
