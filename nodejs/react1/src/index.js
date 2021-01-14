import React from 'react';
import ReactDOM from 'react-dom';
import './index.css';
import App from './App';
import reportWebVitals from './reportWebVitals';
import PropTypes from 'prop-types'

var names = ['Alice', 'Emily', 'Kate'];
var arr = [
  <h1>Hello world!</h1>,
  <h2>React is awesome</h2>,
];

class HelloMessage extends React.Component{
  render() {
    return <h1>Hello {this.props.name}</h1>;
  }
};

class NotesList extends React.Component{
  render() {
    return (
      <ol>
      {
        React.Children.map(this.props.children, function (child) {
          return <li>{child}</li>;
        })
      }
      </ol>
    );
  }
};

class MyTitle extends React.Component{
  render() {
     return <h1> {this.props.title} </h1>;
   }
};

MyTitle.propTypes = {
  title: PropTypes.string.isRequired, 
  //title:React.PropTypes.string.isRequired
}

var data = 123;
ReactDOM.render(
  <MyTitle title={data} />,
  // <NotesList>
  //   <span>hello</span>
  //   <span>world</span>
  //   <span>33</span>
  //   <span>33</span>
  //   <span>wo22rld</span>
  //   <span>qq</span>
  //   <span>ee</span>
  // </NotesList>,
  //  <HelloMessage name="John12313" />,
    //<div>{arr}</div>,
    //<h1>Hello, world!</h1>,
  document.getElementById('root')
);

// If you want to start measuring performance in your app, pass a function
// to log results (for example: reportWebVitals(console.log))
// or send to an analytics endpoint. Learn more: https://bit.ly/CRA-vitals
reportWebVitals();
