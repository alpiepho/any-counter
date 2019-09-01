import React, { useState, useEffect } from "react"
import {
  Button,
  Checkbox,
  Grid,
  FormControlLabel,
  Paper,
  TextField,
  Typography,
} from "@material-ui/core"
import { FaGithub } from "react-icons/fa"
import { makeStyles } from "@material-ui/core/styles"
import {
  js_engine_min,
  js_engine_max,
  js_engine_get,
  js_engine_next,
  js_engine_reset,
} from "./engine"

const useStyles = makeStyles(theme => ({
  root: {
    flexGrow: 1,
  },
  paper: {
    padding: theme.spacing(2),
    textAlign: "center",
    color: theme.palette.text.secondary,
  },
  title: {
    marginTop: 20,
    marginBottom: 5,
  },
  subtitle: {
    marginTop: 0,
    fontSize: 20,
  },
  textField: {
    marginLeft: 30,
    marginBottom: 20,
  },
  textFieldLarge: {
    fontSize: 60,
    marginLeft: 30,
  },
  checkboxField: {
    marginLeft: 20,
    marginBottom: 20,
  },
  gitHub: {
    color: "black",
  },
  button: {
    width: 100,
    marginLeft: 30,
    marginBottom: 20,
  },
  paragraph: {
    margin: 30,
  },
}))

var timerHandle = null

export default function AnyCounter() {
  const [minDigits, setMinDigits] = useState("0 0 0 0 0 0 0 0 0 0")
  const [maxDigits, setMaxDigits] = useState("9 9 9 9 9 9 9 9 9 9")
  const [allDigits, setAllDigits] = useState("0 0 0 0 0 0 0 0 0 0")
  const [running, setRunning] = useState(0)
  const [runRate, setRunRate] = useState(0)
  const [cycles, setCycles] = useState(100)
  const [webAssembly, setWebAssembly] = useState(false)

  const classes = useStyles()

  const isHex = (h)  => {
    return ('abcdef'.includes(h.toLowerCase()))
  }

  const buildArray = (array) => {

    let temp = array.split(' ').map((s) => {
      if (isHex(s)) {
          return parseInt(s,16);
      }
      return Number(s)
    })
    while (temp.length > 10) {
      temp.shift()
    }
    while (temp.length < 10) {
      temp.unshift(0)
    }
    return temp
  }

  const buildWebAssemblyDisplay = ()  => {
    let result = ''
    let i = 0
    for (i=0; i < 10; i++) {
      let d = window.Module.ccall('wa_engine_getN', null, ['number'], [i])
      if (d>9 && d<16)
        result += d.toString(16) + ' '
      else
        result += d.toString(10) + ' '
    }
    return result;
  }

  useEffect(() => {
    if (webAssembly) {
      window.Module.ccall('wa_engine_min', null, ['number','number','number','number','number','number','number','number','number','number'], buildArray(minDigits))
      window.Module.ccall('wa_engine_max', null, ['number','number','number','number','number','number','number','number','number','number'], buildArray(maxDigits))
      window.Module.ccall('wa_engine_reset', null, null, null)
      setAllDigits(buildWebAssemblyDisplay())
    }

    if (!webAssembly) {
      js_engine_min(minDigits)
      js_engine_max(maxDigits)
      js_engine_reset()
      setAllDigits(js_engine_get())
    }
    // eslint-disable-next-line react-hooks/exhaustive-deps
  }, [])

  const stopTimer = () => {
    if (timerHandle) {
      clearInterval(timerHandle)
      timerHandle = null
    }
  }

  const startTimer = () => {
    timerHandle = setInterval(() => {
      if (webAssembly) {
        window.Module.ccall('wa_engine_run', null, ['number'], [cycles])
        setAllDigits(buildWebAssemblyDisplay())
      }

      if (!webAssembly) {
        js_engine_next(cycles)
        setAllDigits(js_engine_get())
      }
    }, runRate)
  }

  const onMinChange = event => {
    setMinDigits(event.target.value)
  }

  const onMaxChange = event => {
    setMaxDigits(event.target.value)
  }

  const onResetClick = () => {

    stopTimer()
    setRunning(false)

    if (webAssembly) {
      window.Module.ccall('wa_engine_min', null, ['number','number','number','number','number','number','number','number','number','number'], buildArray(minDigits))
      window.Module.ccall('wa_engine_max', null, ['number','number','number','number','number','number','number','number','number','number'], buildArray(maxDigits))
      window.Module.ccall('wa_engine_reset', null, null, null)
      setAllDigits(buildWebAssemblyDisplay())
    }

    if (!webAssembly) {
      js_engine_min(minDigits)
      js_engine_max(maxDigits)
      js_engine_reset()
      setAllDigits(js_engine_get())
    }
  }

  const onNextClick = () => {
    if (webAssembly) {
      window.Module.ccall('wa_engine_run', null, ['number'], [1])
      setAllDigits(buildWebAssemblyDisplay())
    }

    if (!webAssembly) {
      js_engine_next(1)
      setAllDigits(js_engine_get())
    }
  }

  const onRunClick = () => {
    if (running) stopTimer()
    else startTimer()
    setRunning(!running)
  }

  const onRunRateChange = event => {
    try {
      if (event.target.value.length) {
        let value = parseInt(event.target.value)
        setRunRate(value)
      }
    } catch (e) {}
  }

  const onCyclesChange = event => {
    try {
      if (event.target.value.length) {
        let value = parseInt(event.target.value)
        setCycles(value)
      }
    } catch (e) {}
  }

  const onWebAssemblyClick = () => {
    setWebAssembly(!webAssembly)
  }

  return (
    <div className={classes.root}>
      <Grid container spacing={3} justify="space-around" direction="column">
        <Grid item>
          <h1 className={classes.title}>Any-Counter</h1>
          <h4 className={classes.subtitle}>
            (counter with variable digit ranges)
          </h4>
        </Grid>

        <Grid item>
          <Paper>
            <h1 className={classes.textFieldLarge}>{allDigits}</h1>
            <Button
              className={classes.button}
              variant="contained"
              color="primary"
              onClick={onNextClick}
            >
              Next
            </Button>
            <Button
              className={classes.button}
              variant="contained"
              color="primary"
              onClick={onRunClick}
            >
              {running ? "Stop" : "Run"}
            </Button>
          </Paper>
        </Grid>

        <Grid item>
          <Paper>
            <Grid item>
              <TextField
                id="standard-helperText"
                label="max"
                defaultValue={maxDigits}
                className={classes.textField}
                helperText="per digit seperated by spaces, hex allowed"
                margin="normal"
                onChange={onMaxChange}
              />
            </Grid>
            <Grid item>
              <TextField
                id="standard-helperText"
                label="min"
                defaultValue={minDigits}
                className={classes.textField}
                helperText="per digit seperated by spaces, hex allowed"
                margin="normal"
                onChange={onMinChange}
              />
            </Grid>
              <Grid item>
                <TextField
                  id="standard-helperText"
                  label="run time"
                  defaultValue={runRate}
                  className={classes.textField}
                  helperText="ms, time delta next calls, 0=fastest"
                  margin="normal"
                  onChange={onRunRateChange}
                />
              </Grid>
              <Grid item>
                <TextField
                  id="standard-helperText"
                  label="cycle count"
                  defaultValue={cycles}
                  className={classes.textField}
                  helperText="Cycles per access"
                  margin="normal"
                  onChange={onCyclesChange}
                />
              </Grid>
              <Grid item>
              <FormControlLabel
                className={classes.checkboxField}
                control={
                  <Checkbox
                    checked={webAssembly}
                    onChange={onWebAssemblyClick}
                    value="checkedB"
                    color="primary"
                  />
                }
                label="WebAssembly"
              />
            </Grid>
            <Grid item>
              <Button
                className={classes.button}
                variant="contained"
                color="primary"
                onClick={onResetClick}
              >
                Reset
              </Button>
            </Grid>
          </Paper>
        </Grid>

        <Grid item>
          <Paper>
            <Typography
              className={classes.paragraph}
              variant="body1"
              gutterBottom
            >
              Why? What is the purpose of this application?
            </Typography>

            <Typography
              className={classes.paragraph}
              variant="body1"
              gutterBottom
            >
              To find out, and learn more about how this uses WebAssembly, please see
              the details on GitHub{" "}
              <a
                href={`https://github.com/alpiepho/any-counter`}
                target="_blank"
                rel="noopener noreferrer"
                aria-label="github"
              >
                <FaGithub
                  className={classes.gitHub}
                  size={32}
                  aria-hidden="true"
                />
              </a>
            </Typography>
          </Paper>
        </Grid>
      </Grid>
    </div>
  )
}
