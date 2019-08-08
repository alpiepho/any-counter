import React from "react"
import { Link } from "gatsby"

import Layout from "../components/layout"
import SEO from "../components/seo"
import AnyCounter from "../components/anycounter"

const IndexPage = () => (
  <Layout>
    <SEO title="Home" />
    <AnyCounter />
  </Layout>
)

export default IndexPage
