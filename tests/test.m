[class] C1
[property] C1::toast
[class] C2
[property] C2::toast
[category] C3
[property] C3::toast
[protocol] P4
[property] P4::toast
[implementation] C1
[method] C1::- (void)doSomething:(id)x
[class_method] C1::+ (void)doSomething:(id)x
[category_implementation] C2
[method] C2::- (void)doSomething:(id)x
[class_method] C2::+ (void)doSomething:(id)x
%%%
@interface C1 : SC1 {
}
@property (strong) id toast;
@end

@interface C2 : SC1
@property (strong) id toast;
@end

@interface C3 (SomeCategory)
@property (strong) id toast;
@end

@protocol P4
@property (strong) id toast;
@end

@implementation C1
- (void)doSomething:(id)x {

}
+ (void)doSomething:(id)x {

}
@end
@implementation C2 (SomeCategory)
- (void)doSomething:(id)x {

}
+ (void)doSomething:(id)x {

}
@end
