import React, { useState, useEffect } from "react"
import { Button, Grid, TextField } from "@material-ui/core"
import { makeStyles } from "@material-ui/core/styles"
import {
  engine_min,
  engine_max,
  engine_get,
  engine_add,
  engine_sub,
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
  textFieldLarge: {
    fontSize: 60,
  },
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
  },[])

  const onMinChange = event => {
    setMinDigits(event.target.value)
  }

  const onMaxChange = event => {
    setMaxDigits(event.target.value)
  }

  const onResetClick = () => {
    console.log('onResetClick')
    engine_min(minDigits)
    engine_max(maxDigits)
    engine_reset()
    setAllDigits(engine_get())
  }

  const onAddClick = () => {
    console.log('onAddClick')
    engine_add()
    setAllDigits(engine_get())
  }

  const onSubClick = () => {
    engine_sub()
    setAllDigits(engine_get())
  }

  return (
    <div className={classes.root}>
      <Grid container spacing={3}>
        <Grid item xs={12}>
          <h1 className={classes.title}>Any-Counter</h1>
          <h4 className={classes.subtitle}>
            (counter with variable digit ranges)
          </h4>
        </Grid>
        <Grid item xs={5}>
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
        <Grid item xs={5}>
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
        <Grid item xs={2}>
          <Button variant="contained" color="primary" onClick={onResetClick}>
            Reset
          </Button>
        </Grid>

        <Grid item xs={10}>
          <h1 className={classes.textFieldLarge}>{allDigits}</h1>
        </Grid>
        <Grid item xs={2}>
          <Grid container spacing={3} direction="column">
            <Grid item>
              <Button variant="contained" color="primary" onClick={onAddClick}>
                Add
              </Button>
            </Grid>
            <Grid item>
              <Button variant="contained" color="primary" onClick={onSubClick}>
                Sub
              </Button>
            </Grid>
          </Grid>
        </Grid>
      </Grid>
    </div>
  )
}
