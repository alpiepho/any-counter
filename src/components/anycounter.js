import React, { useState, useEffect } from "react"
import { Button, Grid, Paper, TextField, Typography } from "@material-ui/core"
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
  gitHub: {
    color: "black",
  },
  button: {
    width: 100,
    marginLeft: 30,
    marginBottom: 20,
  },
  paragraph: {
    margin: 30
  }
}))

export default function AnyCounter() {
  const [minDigits, setMinDigits] = useState("0 0")
  const [maxDigits, setMaxDigits] = useState("1 1")
  const [allDigits, setAllDigits] = useState("0 0")

  const classes = useStyles()

  useEffect(() => {
    engine_min(minDigits)
    engine_max(maxDigits)
    engine_reset()
    setAllDigits(engine_get())
  }, [])

  const onMinChange = event => {
    setMinDigits(event.target.value)
  }

  const onMaxChange = event => {
    setMaxDigits(event.target.value)
  }

  const onResetClick = () => {
    engine_min(minDigits)
    engine_max(maxDigits)
    engine_reset()
    setAllDigits(engine_get())
  }

  const onNextClick = () => {
    engine_next()
    setAllDigits(engine_get())
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
            <Typography className={classes.paragraph} variant="body1" gutterBottom>
              Why? What is the purpose of this application?
            </Typography>
            <Typography className={classes.paragraph}  variant="body1" gutterBottom>
              There are several problems that can be solved with a counter that
              uses different ranges for the digits. For example, if you enter
              all 1s for max, then you have a binary counter. If you enter 'FF',
              then you have a hex counter.
            </Typography>
            <Typography className={classes.paragraph}  variant="body1" gutterBottom>
              A more complex example is min set to '5 0 0' and max set to '9 59
              59' then you have a time counter from 5 mins to 10 mins.
            </Typography>
            <Typography className={classes.paragraph}  variant="body1" gutterBottom>
              This algorithm was originally used to 'count' thru the
              permutations of letter from a phone. '2' shows 'abc', '7' shows
              'pqrs'. The ranges are different. This algorithm was used to map
              from integers to letters. With min set to '0 0', and max to '2 3',
              the count would be '0 0', '0 1'...'2 3'. This can easily be mapped
              to 'ap', 'aq'...'cs'.
            </Typography>
            <Typography className={classes.paragraph}  variant="body1" gutterBottom>
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
