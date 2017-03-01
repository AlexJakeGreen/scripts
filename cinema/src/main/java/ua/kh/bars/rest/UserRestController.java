package ua.kh.bars.rest;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.RestController;
import ua.kh.bars.entities.User;
import ua.kh.bars.services.UserService;

/**
 * Created by green on 4/13/16.
 */

@RestController
@RequestMapping("/rest")
public class UserRestController {

    @Autowired
    UserService userService;

    @RequestMapping(method = RequestMethod.GET)
    public User getUser(@RequestParam("id") Long id) {
        User user = userService.findOne(id);
        return user;
    }
}
