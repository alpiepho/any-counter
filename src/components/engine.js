

var minHex = false
var maxHex = false
var minDigits = [0]
var maxDigits = [0]
var allDigits = [0]

function counter_increment(){
    let place = allDigits.length - 1;

    allDigits[place] += 1
    // work backwards, add 1 to buffer[col][n] and carry if needed to n-1
    while (allDigits[place] > maxDigits[place]) {
        allDigits[place] = 0 // reset place
        if (place === 0)
            break;         // have we maxed out
        if (place > 0) allDigits[place-1] += 1 // carry to next plae
        place -= 1;
    }
}

function isHex(h) {
    return ('abcdef'.includes(h.toLowerCase()))
}

function asHex(d) {
    if (minHex | maxHex) {
        if (d>9 && d<16) return d.toString(16)
    }
    return d.toString(10)
}

export function engine_min(newValue) {
    minHex = false
    minDigits = newValue.split(' ').map((s) => {
        if (isHex(s)) {
            minHex = true
            return parseInt(s,16);
        }
        return Number(s)
    })
}

export function engine_max(newValue) {
    maxHex = false
    maxDigits = newValue.split(' ').map((s) => {
        if (isHex(s)) {
            maxHex = true
            return parseInt(s,16);
        }
        return Number(s)
    })
}

export function engine_get() {
    let result = allDigits.reduce((prev, digit) => {
        return prev + ' ' + asHex(digit)
    }, '')
    return result
}

export function engine_next() {
    counter_increment()
}

export function engine_reset() {
    allDigits = [...minDigits]
}
