/**
 * Driver registry is used to store all the Driver classes.
 *
 * Each Driver class needs to register itself to the registry.
 * This is done using the `registerDriverClass` function.
 *
 * The factory function `getDriverClass` is used to retrieve a specific
 * Driver class from the registry.
 *
 */
const driverClasses = {};

/**
 * Retrieves the Driver class with the specified name.
 *
 * @param {string} driverClassName - The name of the Driver class.
 * @return {function|null} The Driver class if found, otherwise null.
 */
function getDriverClass(driverClassName) {
    return driverClasses.hasOwnProperty(driverClassName) ? driverClasses[driverClassName] : null;
}

/**
 * Registers a Driver class to the Driver registry.
 *
 * @param {function} driverClass - The Driver class to be registered.
 * @param {string} [alias] - An optional alias for the Driver class.
 * @return {function|null} The registered Driver class or null if the Driver class doesn't have a name.
 */
function registerDriverClass(driverClass, alias) {
    if (typeof driverClass.name === "undefined") {
        console.error("Driver class must have a name");
        return null;
    }

    driverClasses[driverClass.name] = driverClass;
    if (alias) {
        driverClasses[alias] = driverClass;
    }
}

module.exports = {
    driverClasses,
    getDriverClass,
    registerDriverClass
};
