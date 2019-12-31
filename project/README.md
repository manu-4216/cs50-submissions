# Mayurpankh Events


### Hosted at: https://mayurpankh.netlify.com/

### Github url: https://github.com/manu-4216/mayurpankh-events

## Context
I decided to build this project to help a family member have visibility about her activity - promoting Indian classical artists and artisans.

The technical choices that I made are oriented towards having a fast website, without using payed services, other than the custom domain name.


At the same time, this project is useful for my own development, by applying React, GraphQL, learning about headless CMS options.
Secondly, interacting with a real client, in a realistic manner helped me also improve my soft skills.


## Technical details:
- built with Gatsby, using the default barebones starter. Gatsby is a static site generator, which builds fast websites based on static pages.
- hosted on Netlify, linked to the corresponding github repo
- it uses React components, with CSS-modules, built from scratch, without any UI components library

## Other details about my own implementation of this project:
- it uses custom-built from scratch (no UI components library) in React
- it uses the barebones 'gatsby-default_starter' Gatsby starter
-

## State of the project:

- finished parts:
    - pages: home page, about, contact, sponsors, features
    - deployment on Netlify

- work in progress parts:
    - adding extra content on the home page: a list of 4 most important characteristics of the company

- comming soon parts, needed for the MVP:
    - buying a custom domain for deployment
    - some pages such as: galery, past+upcoming events
    - connect it to Dropbox, using 'gatsby-source-dropbox', and use it as a headless CMS provider for:
        - markdown files to describe structured data, such as past events
        - media provider for galery images
        - webhook to trigger a new build when changes are being made in the Dropbox folder

- later-on planned parts:
    - UI refinements
    - SEO improvements
    - custom

## What I learned, but it isn't included in the final project:
- all reputed headless CMS options aren't free. I looked into Netlify, Contentful, and Strapi for instance to use as a data layer, but their free version seemed to be limiting, after a certain point, so I decided to not use it anymore, as I was initially estimating.
- I tried Strapi, which is open source, but I found it unpractical to use for my use-case, so I ended up dropping it
- I looked into connecting Gatsby to Dropbox, and I was succesful in linking the 2. This is what I decided to use, but I didn't get to it yet, as other parts need more work first.


