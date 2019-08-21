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
  engine_min,
  engine_max,
  engine_get,
  engine_next,
  engine_reset,
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
  const [minDigits, setMinDigits] = useState("0 0 0 0")
  const [maxDigits, setMaxDigits] = useState("9 9 9 9")
  const [allDigits, setAllDigits] = useState("0 0 0 0")
  const [running, setRunning] = useState(0)
  const [runRate, setRunRate] = useState(1000)
  const [cycles, setCycles] = useState(1000)
  const [webAssembly, setWebAssembly] = useState(false)

  const classes = useStyles()

  useEffect(() => {
    engine_min(minDigits)
    engine_max(maxDigits)
    engine_reset()
    setAllDigits(engine_get())
  }, [])

  const stopTimer = () => {
    if (timerHandle) {
      clearInterval(timerHandle)
      timerHandle = null
    }
  }

  const startTimer = () => {
    timerHandle = setInterval(() => {
      engine_next()
      setAllDigits(engine_get())
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
    engine_min(minDigits)
    engine_max(maxDigits)
    engine_reset()
    setAllDigits(engine_get())
  }

  const onNextClick = () => {
    engine_next()
    setAllDigits(engine_get())
  }

  const onRunClick = () => {
    if (running) stopTimer()
    else {
      startTimer()
    }
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
            {!webAssembly ? (
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
            ) : null}
            {webAssembly ? (
              <Grid item>
                <TextField
                  id="standard-helperText"
                  label="cycle count"
                  defaultValue={cycles}
                  className={classes.textField}
                  helperText="WebAssembly Cycles per access"
                  margin="normal"
                  onChange={onCyclesChange}
                />
              </Grid>
            ) : null}
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
              There are several problems that can be solved with a counter that
              uses different ranges for the digits. For example, if you enter
              all 1s for max, then you have a binary counter. If you enter 'FF',
              then you have a hex counter.
            </Typography>
            <Typography
              className={classes.paragraph}
              variant="body1"
              gutterBottom
            >
              A more complex example is min set to '5 0 0' and max set to '9 59
              59' then you have a time counter from 5 mins to 10 mins.
            </Typography>
            <Typography
              className={classes.paragraph}
              variant="body1"
              gutterBottom
            >
              The 'run' feature lets you free run the counter. The 'run rate'
              should let you guage the difference in
              performance of the Javascript implentation vs the WebAssembly
              version.  WebAssembly uses a cycle count since there is no timer 
              in WebAssembly (that I know of).
            </Typography>
            <Typography
              className={classes.paragraph}
              variant="body1"
              gutterBottom
            >
              This example can be the source of other various types of counters.
              The source can be found on GitHub{" "}
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
