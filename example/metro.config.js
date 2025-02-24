const path = require("path");
const { getDefaultConfig, mergeConfig } = require("@react-native/metro-config");
const { getConfig } = require("react-native-builder-bob/metro-config");
const pkg = require("../package.json");

const root = path.resolve(__dirname, "..");

/**
 * Metro configuration
 * https://facebook.github.io/metro/docs/configuration
 *
 * @type {import('metro-config').MetroConfig}
 */

const config = {
  resolver: {
    unstable_enablePackageExports: true,
  },
};

module.exports = getConfig(mergeConfig(getDefaultConfig(__dirname), config), {
  root,
  pkg,
  project: __dirname,
});
